/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:01:22 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/17 02:03:30 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 전역변수는 시그널을 전역변수로 두어야합니다.

void	leaks()
{
	system("leaks a.out");
}

char	**copy_env(char **env)
{
	char	**tmp;
	int		i;

	i = 0;
	while (env[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tmp)
		return (NULL);
	i = -1;
	while (env[++i])
		tmp[i] = ft_strdup(env[i]);
	tmp[i] = NULL;
	return (tmp);
}

int	check_whitespace(const char *command)
{
	int			i;

	i = 0;
	while (command[i])
	{
		if (command[i] != 32 && !(command[i] >= 9 && command[i] <= 13))
			return (0);
		i++;
	}
	return (1);
}
// 더블쿼터와 = 변수가 처리가됩니다
// 싱글쿼터  = 변수처리가 되지않습니다

int	return_error(char *msg, int error_code)
{
	ft_putstr_fd(msg, 2);
	return (error_code);
}

void	push_tmp(t_tmp **list, char *data)
{
	t_tmp	*tmp;
	t_tmp	*head;

	tmp = (t_tmp *)malloc(sizeof(t_tmp));
	tmp->data = data;
	tmp->next = NULL;
	if (*list == NULL)
		*list = tmp;
	else
	{
		head = *list;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

void	*free_split(char **base)
{
	int	i;

	i = -1;
	while (base[++i])
		free(base[i]);
	free(base);
	return (NULL);
}

t_tmp *make_cmd_list(char **tmp1, t_table *table)
{
	int			i;
	int			j;
	char		**tmp2;
	t_tmp		*list;

	i = -1;
	list = NULL;
	while (tmp1[++i])
	{
		if (tmp1[i][0] == '\"' || tmp1[i][0] == '\'')
		{
			push_tmp(&list, ft_strdup(tmp1[i]));
			continue ;
		}
		tmp2 = ft_split_operator(tmp1[i], " ");
		if (!tmp2 || table->syntax_error)
			return (free_split(tmp2), free_split(&tmp1[i]));
		j = -1;
		while (tmp2[++j])
			push_tmp(&list, ft_strdup(tmp2[j]));
		free_split(tmp2);
	}
	return (list);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (0);
}

int	get_cmd_type(char *cmd)
{
	if (!ft_strcmp(">>", cmd))
		return (redict_out);
	if (!ft_strcmp("<<", cmd))
		return (redict_in);
	if (!ft_strcmp(">", cmd))
		return (dict_out);
	if (!ft_strcmp("<", cmd))
		return (dict_in);
	if (!ft_strncmp("\"", cmd, 1))
		return (dquoute);
	if (!ft_strncmp("\'", cmd, 1))
		return (quote);
	if (!ft_strcmp("|", cmd))
		return (pipe_);
	if (!ft_strncmp("-", cmd, 1))
		return (option);
	return (argv);
}

void	push_cmd(t_cmd_info **cmd, char *data, int type)
{
	t_cmd_info *tmp;
	t_cmd_info *head;

	tmp = (t_cmd_info *)malloc(sizeof(t_cmd_info));
	tmp->data = data;
	tmp->type = type;
	tmp->next = NULL;
	if (*cmd == NULL)
		*cmd = tmp;
	else
	{
		head = *cmd;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

int	cmd_size(t_table *table, t_tmp *list)
{
	int	count;

	count = 1;
	while (list)
	{
		if (!ft_strcmp(list->data, "|"))
			count++;
		list = list->next;
	}
	return (count);
}

int	check_operator(int type)
{
	if ((type == redict_out || type == redict_in) || \
		(type == dict_in || type == dict_out))
		return (1);
	else if (type == pipe_)
		return (2);
	return (0);
}

t_cmd_info	**set_cmd_list(t_table *table, t_tmp *list)
{
	int	i;
	t_cmd_info	**tmp;

	table->cmd_count = cmd_size(table, list);
	tmp = (t_cmd_info **)malloc(sizeof(t_cmd_info *) * (table->cmd_count + 1));
	tmp[table->cmd_count] = NULL;
	i = -1;
	while (++i < table->cmd_count)
		tmp[i] = NULL;	
	return (tmp);
}

int	check_unexpect_operator(t_tmp **list, t_table *table)
{
	*list = (*list)->next;
	if (!*list)
	{
		table->syntax_error = 2;
			return (1);
	}
	if (check_operator(get_cmd_type((*list)->data)))
		table->syntax_error = 1;
	return (0);
}

t_cmd_info	**syntax_interpretation(t_tmp *list, t_table *table)
{
	t_cmd_info	**tmp;
	int			type;
	int			i;

	i = 0;
	tmp = set_cmd_list(table, list);
	while (list)
	{
		type = get_cmd_type(list->data);
		if (check_operator(type) == 1)
			if (check_unexpect_operator(&list, table))
				break ;
		if (type != pipe_)
			push_cmd(&tmp[i], list->data, type);
		else
		{
			if (!list->next)
				table->syntax_error = 1;
			i++;
		}
		list = list->next;
	}
	return (tmp);
}

void	replace_argv_to_command(t_cmd_info **cmd)
{
	int	i;
	t_cmd_info	*head;

	i = 0;
	while (cmd[i])
	{
		head = cmd[i];
		while (cmd[i])
		{
			if (cmd[i]->type == argv)
			{
				cmd[i]->type = command;
				break ;
			}
			cmd[i] = cmd[i]->next;
		}
		cmd[i] = head;
		i++;
	}
}

t_cmd_info	**parse(char *command_line, t_table *table)
{
	char		**tmp1;
	t_tmp		*list;
	t_cmd_info	**node;
	t_cmd_info	*head;

	tmp1 = ft_split_quote(command_line, " ", table);
	table->split_tmp = tmp1;
	if (table->syntax_error)
	{
		free_split(tmp1);
		table->exit_status = return_error("minishell: syntax error\n", 258);
		return (NULL);
	}
	list = make_cmd_list(tmp1, table);
	table->node = list;
	if (!list)
	{
		table->exit_status = return_error("minishell: syntax error\n", 258);
		return (NULL);
	}
	node = syntax_interpretation(list, table);
	if (table->syntax_error == 1)
	{
		table->exit_status = return_error("minishell: syntax error\n", 258);
		//return (NULL);
	}
	replace_argv_to_command(node);
	int i = 0;
	char	*arr[100] = {
		"command",
		"option",
		"argv",
		"string",
		"redict_out",
		"redict_in",
		"dict_out",
		"dict_in",
		"dquoute",
		"quote",
		"pipe"
	};
	while (node[i])
	{
		head = node[i];
		while (node[i])
		{
			printf("type : %s:%s\n", arr[node[i]->type], node[i]->data);
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
	if (table->syntax_error == 2)
		table->exit_status = return_error("minishell: syntax error\n", 258);
	return (node);
	// 명령어 상태를 점검합니다 --> 만일 | > >> 뒤에 unexpect인자가 들어올 경우 syntax flag를 사용하여 에러처리 후 탈출합니다
	//check_operator(cmd, table);
	// 쿼터체크
	// 명령어 토근화
}



int	main(int ac, char *av[], char *env[])
{
	t_table		table;
	t_cmd_info	**cmd;
	char		*input_command;

	table.envp = copy_env(env);  // ---> 환경변수를 복사해둠으로써 팀원도 편하게 자원을 사용할 수 있도록 유도합니다
	while (ac && av)
	{
		input_command = readline("minishell$ "); // ---> 명령어 입력을 받도록합니다.
		if (!input_command)
			break ;
		if (*input_command != '\0' && !check_whitespace(input_command))
		{
			add_history(input_command); // 명령어를 기록하는 과정을 거칩니다 ----> history에 쌓이는 순사가 있을까요??
			cmd = parse(input_command, &table);
			//table.exit_status = execute();
		}
		free(input_command);
	}
	//ft_putstr_fd("\x1b[1A", STDOUT_FILENO);
	ft_putstr_fd("\033[2D", STDOUT_FILENO);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	//atexit(leaks);
	exit (0);
}

// >> , > 무조건 뒤에 인자가 와야함
// 명령어는 더블 쿼터로 감쌀 수 있다
// 파이프 리다이렉션 다이렉션은 문자열로 감싸질 수 없다.
// 명령어 뒤에 파이프만 온 경우에 쉘 입력모드로 전환된다.
// 리다이렉션 뒤에는 무조건 파일이름이 와야합니다 파이프 리다이렉션 다이렉션은 올 수 없습니다