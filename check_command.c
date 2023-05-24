/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:01:22 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/25 01:33:51 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
char	*replace_val(t_cmd_info	*node, t_table *table);

// 'rl_replace_line' 함수의 선언
void rl_replace_line(const char *text, int clear_undo);

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
	if (!base)
		return (NULL);
	while (base[++i])
		free(base[i]);
	free(base);
	return (NULL);
}

t_tmp	*make_cmd_list(char **tmp1, t_table *table)
{
	int			i;
	int			j;
	char		**tmp2;
	t_tmp 		*list;

	i = -1;
	list = NULL;
	if (!tmp1)
		return (NULL);
	while (tmp1[++i])
	{
		tmp2 = ft_split_operator(tmp1[i], "");
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
		return (dquote);
	if (!ft_strncmp("\'", cmd, 1))
		return (quote);
	if (!ft_strcmp("|", cmd))
		return (pipe_);
	if (!ft_strncmp("-", cmd, 1))
		return (option);
	if (!ft_strcmp("(", cmd))
		return (unexpect_token);
	if (!ft_strcmp(";", cmd))
		return (unexpect_token);
	if (!ft_strcmp(")", cmd))
		return (unexpect_token);
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
	if (!list)
		return (NULL);
	tmp = set_cmd_list(table, list);
	while (list)
	{
		type = get_cmd_type(list->data);
		if (check_operator(type) == 1)
			if (check_unexpect_operator(&list, table))
				break ;
		if (type != pipe_)
			push_cmd(&tmp[i], ft_strdup(list->data), type);
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
	if (!cmd)
		return ;
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

void	print_cmd(t_cmd_info **node)
{
	int i = 0;
	t_cmd_info	*head;
	char	*arr[100] = {
		"command",
		"option",
		"argv",
		"string",
		"redict_out",
		"redict_in",
		"dict_out",
		"dict_in",
		"dquote",
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
}

void	write_heredoc(char *line, t_table *table, int infile)
{
	t_cmd_info	node;
	char		*tmp;

	node.data = line;
	node.data = replace_val(&node, table);
	if (!node.data)
	{
		tmp = ft_strjoin(line, "\n");
		write(infile, tmp, ft_strlen(tmp));
		free(tmp);
	}
	else
	{
		tmp = ft_strjoin(node.data, "\n");
		write(infile, tmp, ft_strlen(tmp));
		free(node.data);
		free(tmp);
	}	
}

int	init_here_doc_data(char *limits, t_table *table)
{
	int		fd;
	char	*line;
	int		infile;

	fd = open("/tmp/sh-thd-1641928925", \
	O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	infile = dup(fd);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limits) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc(line, table, infile);
		free(line);
	}
	close(infile);
	infile = open("/tmp/sh-thd-1641928925", O_RDONLY);
	close(fd);
	unlink("/tmp/sh-thd-1641928925");
	return (infile);
}

void	push_fd(t_command **cmd_list, int infile, int outfile)
{
	t_command	*tmp;
	t_command	*head;

	tmp = (t_command *)malloc(sizeof(t_command));
	tmp->infile = infile;
	tmp->outfile = outfile;
	tmp->next = NULL;
	if (!*cmd_list)
		*cmd_list = tmp;
	else
	{
		head = *cmd_list;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

void	replace_fd(t_command *cmd_list, int infile, int outfile)
{
	while (cmd_list->next)
		cmd_list = cmd_list->next;
	cmd_list->infile = infile;
	cmd_list->outfile = outfile;
}

void	errno_print(const char *str, t_table *table)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	table->exit_status = 1;
	table->fd_status = -1;
}

void	close_file(t_cmd_info *node, t_fd_status *fd)
{
	if (node->type == redict_in)
	{
		if (fd->in != 0)
			close(fd->in);
	}
	if (node->type == dict_in && fd->status != -1)
	{
		if (fd->in != 0)
			close(fd->in);
	}
	if (node->type == redict_out && fd->status != -1)
	{
		if (fd->out != 1)
			close(fd->out);
	}
	if (node->type == dict_out && fd->status != -1)
	{
		if (fd->out != 1)
			close(fd->out);
	}
}

void	open_in_out_file(t_cmd_info **node, t_fd_status *fd,\
 int i, t_heredoc_fd **h_fd)
{
	while (node[i])
	{
		close_file(node[i], fd);
		if (node[i]->type == redict_in)
		{
			fd->in = (*h_fd)->fd;
			*h_fd = (*h_fd)->next;
		}
		if (node[i]->type == dict_in && fd->status != -1)
		{
			fd->in = open(node[i]->data, O_RDONLY);
			if (fd->in == -1)
			{
				fd->error_file = node[i]->data;
				fd->status = -1;
			}
		}
		if (node[i]->type == redict_out && fd->status != -1)
			fd->out = open(node[i]->data, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (node[i]->type == dict_out && fd->status != -1)
			fd->out = open(node[i]->data, O_RDWR | O_CREAT | O_TRUNC, 0644);
		node[i] = node[i]->next;
	}
}

t_command	*check_in_out_file(t_cmd_info **node, t_heredoc_fd *h_fd, t_table *table)
{
	int 			i;
	t_fd_status		fd;
	t_cmd_info		*head;
	t_command		*cmd_list;
	t_heredoc_fd	*tmp;

	i = -1;
	cmd_list = NULL;
	fd.status = 0;
	tmp = h_fd;
	while (node[++i])
	{
		head = node[i];
		fd.in = 0;
		fd.out = 1;
		push_fd(&cmd_list, 0, 1);
		open_in_out_file(node, &fd, i, &h_fd);
		if (fd.status == -1)
		{
			errno_print(fd.error_file, table);
			fd.status = 0;
		}
		replace_fd(cmd_list, fd.in, fd.out);
		node[i] = head;
	}
	h_fd = tmp;
	return (cmd_list);
}

int	count_command(t_cmd_info *node)
{
	int	i;

	i = 0;
	while (node)
	{
		if (node->type == command || node->type  == option\
		|| node->type == argv || node->type == dquote\
		|| node->type == quote)
			i++;
		node = node->next;
	}
	return (i);
}

char	**set_cmd(t_cmd_info *node)
{
	int		size;
	char	**arr;
	int		i;
	size = count_command(node);
	if (size == 0)
		return (NULL);
	else
		arr = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (i <= size)
	{
		arr[i] = NULL;
		i++;
	}
	return (arr);
}

void	make_command(t_command *cmd_list, t_cmd_info **node)
{
	int	i;
	int	count;
	int	size;
	t_cmd_info	*head;

	i = 0;
	while (node[i])
	{
		head = node[i];
		cmd_list->cmd = set_cmd(node[i]);
		count = 0;
		cmd_list->num_of_cmd = i;
		while (node[i])
		{
			if (node[i]->type == command || node[i]->type  == option\
			|| node[i]->type == argv || node[i]->type == dquote\
			|| node[i]->type == quote)
			{
				cmd_list->cmd[count] = ft_strdup(node[i]->data);
				count++;
			}
			node[i] = node[i]->next;
		}
		cmd_list = cmd_list->next;
		node[i] = head;
		i++;
	}
}

int	env_len(char *str, t_table *table)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	str = str + 1;
	if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '?' || str[i] == '\"' || str[i] == '\'')
	{
		while (str[i] && str[i] != 32 && !(str[i] > 9 && str[i] < 13) && str[i] != '$')
		{
			if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'z')\
			|| (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '?'))
				return (i);
			i++;
		}
	}
	else if (str[i] == 32 || (str[i] > 9 && str[i] < 13))
		return (0);
	else
		table->syntax_error = 1;
	return (i);	
}

void	*free_return_null(t_replace *d)
{
	if (!d)
		return (NULL);
	free(d->ret);
	return (NULL);
}

char	*replace_val2(t_replace *d, int len)
{
	if (len == -1 && (!d->base || !*(d)->base))
		return (d->ret);
	d->tail = ft_strdup(d->base);
	d->tmp = ft_strjoin(d->ret, d->tail);
	free(d->tail);
	free(d->ret);
	return (d->tmp);
}

void	replace_val3(t_replace *d, t_table *table)
{
	if (!ft_strcmp(d->tar, "?"))
	{
		free(d->tar);
		d->tar = ft_itoa(table->exit_status);
		return ;
	}
	d->tmp2 = ft_strjoin(d->tar, "=");
	free(d->tar);
	d->tar = getenv(d->tmp2);
	free(d->tmp2);
	d->tmp2 = NULL;
	if (!d->tar)
		d->tar = ft_strdup("");
	else
		d->tar = ft_strdup(d->tar);
}

void	replace_val4(t_replace *d, int len)
{
	d->tmp2 = ft_strjoin(d->head, d->tar);
	free(d->head);
	free(d->tar);
	d->tmp3 = d->ret;
	d->ret = ft_strjoin(d->tmp3, d->tmp2);
	free(d->tmp2);
	free(d->tmp3);
	if (len == 0)
	{
		d->tmp2 = ft_strjoin(d->ret, "$");
		free(d->ret);
		d->ret = d->tmp2;
	}
	if (ft_strlen(d->tmp) >= len + 1)
		d->base = d->tmp + len + 1;
	else
		d->base = NULL;
}

int	check_is_quote(char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\"' || tmp[i] == '\'')
			break ;
		i++;
	}
	if (tmp[i] && tmp[i] == '\'')
		return (0);
	return (1);
}

char	*replace_val(t_cmd_info	*node, t_table *table)
{
	t_replace	d;
	int			len;
	int			i;

	d.base = node->data;
	i = 0;
	if (!check_is_quote(node->data))
		return (NULL);
	d.ret = ft_strdup("");
	while (1)
	{
		d.tmp = ft_strchr(d.base, '$');
		if (i == 0 && (!(d.tmp) || !*(d.tmp + 1)))
			return (free_return_null(&d));
		len = env_len(d.tmp, table);
		if (table->syntax_error)
			return (NULL);
		if (!d.tmp)
			return (replace_val2(&d, len));
		d.head = ft_substr(d.base, 0, d.tmp - d.base);
		d.tar = ft_substr(d.tmp + 1, 0, len);
		if (len != 0)
			replace_val3(&d, table);
		replace_val4(&d, len);
		i++;
	}
}

void	replace_environment_variable(t_cmd_info	**node, t_table *table)
{
	int	i;
	int	j;
	char		*tmp;
	char		*tmp2;
	t_cmd_info	*head;

	i = 0;
	j = 0;
	while (node[i])
	{
		head = node[i];
		while (node[i])
		{
			tmp = replace_val(node[i], table);
			if (tmp && node[i]->type != quote)
			{
				free(node[i]->data);
				node[i]->data = tmp;
			}
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}

char	*remove_env_dquote(char *tmp)
{
	int	i;
	char	*ret;

	i = 0;
	if (!tmp[i])
		return (NULL);
	if (get_cmd_type(tmp) == quote)
		return (NULL);
	if (tmp[i] == '$' && (tmp[i + 1] == '\"' || tmp[i + 1] == '\''))
	{
		ret = ft_substr(&tmp[i + 2], 0, ft_strlen(&tmp[i + 2]) - 1);
		free(tmp);
		return (ret);
	}
	return (NULL);
}

int	check_dq(char *tmp)
{
	int	i;

	i = 2;
	while (tmp[i] && tmp[i] != '\"' && tmp[i] != '\'')
		i++;
	if (!tmp[i])
		return (0);
	else
		return (1);
}

char	*remove_env_dquote_2(char **base)
{
	int	i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	tmp = *base;
	if (!tmp[i])
		return (tmp);
	if (get_cmd_type(tmp) == quote)
		return (tmp);
	tmp2 = remove_env_dquote(tmp);
	if (tmp2)
		return (tmp2);
	while (tmp[i + 1])
	{
		if (tmp[i] == '$' && (tmp[i + 1] == '\"'))
		{
			if (i > 0 && (tmp[i - 1] == '\"' || tmp[i - 1] == '\''))
				tmp2 = ft_strdup("$");
			else
			{
				if (check_dq(&tmp[i]))
					tmp2 = ft_substr(&tmp[i + 2], 0, ft_strlen(&tmp[i + 2]) - 1);
				else
					break ;
			}
			free(tmp);
			return (tmp2);
		}
		i++;
	}
	return (tmp);
}

void	link_quote(t_cmd_info **node, char **tmp)
{
	int	i;
	char	*ret;
	char	*buff;

	i = 0;
	free((*node)->data);
	ret = ft_strdup("");
	while (tmp[i])
	{
		buff = ft_strjoin(ret, tmp[i]);
		free(ret);
		ret = buff;
		i++;
	}
	(*node)->data = ret;
}

void	*remove_if(t_cmd_info *node, t_table *table)
{
	char	*base;
	char	**tmp;
	char	*ret;
	int		i;

	base = node->data;
	tmp = ft_split_divid_quote(base, "", table);
	i = 0;
	while (tmp[i])
	{
		tmp[i] = remove_env_dquote_2(&tmp[i]);
		if (get_cmd_type(tmp[i]) == dquote || get_cmd_type(tmp[i]) == quote)
		{
			ret = ft_substr(tmp[i], 1, ft_strlen(tmp[i]) - 2);
			free(tmp[i]);
			tmp[i] = ret;
		}
		i++;
	}
	link_quote(&node, tmp);
	return (free_split(tmp));
}

void	remove_dquote(t_cmd_info **node, t_table *table)
{
	int	i;
	int	j;
	char		*tmp;
	char		*tmp2;
	t_cmd_info	*head;

	i = 0;
	j = 0;
	while (node[i])
	{
		head = node[i];
		while (node[i])
		{
			remove_if(node[i], table);
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}

void	push_heredoc_fd(t_heredoc_fd **h_fd, int fd)
{
	t_heredoc_fd	*tmp;
	t_heredoc_fd	*head;

	tmp = (t_heredoc_fd *)malloc(sizeof(t_heredoc_fd));
	tmp->fd = fd;
	tmp->next = NULL;
	if (*h_fd == NULL)
		*h_fd = tmp;
	else
	{
		head = *h_fd;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

t_heredoc_fd	*check_heredoc(t_cmd_info **node, t_table *table)
{
	int	i;
	int	fd;
	t_cmd_info		*head;
	t_heredoc_fd	*h_fd;

	i = 0;
	h_fd = NULL;
	while (node[i])
	{
		head = node[i];
		fd = 0;
		while (node[i])
		{
			if (node[i]->type == redict_in)
			{
				fd = init_here_doc_data(node[i]->data, table);
				push_heredoc_fd(&h_fd, fd);
			}
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
	return (h_fd);
}

void	check_syntax_error(t_cmd_info **node, t_table *table)
{
	int			i;
	int			j;
	t_cmd_info	*head;

	i = 0;
	if (!node)
		return ;
	while (node[i])
	{
		head = node[i];
		while (node[i])
		{
			if (node[i]->type != dquote && node[i]->type != quote)
			{
				j = 0;
				while (node[i]->data[j])
				{
					if (node[i]->data[j] == ')' || node[i]->data[j] == '(' \
					|| node[i]->data[j] == ';')
					{
						table->syntax_error = 1;
						node[i] = head;
						return ;
					}
					j++;
				}
			}
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}

void	print_cmd2(t_command *cmd_list)
{
	while (cmd_list)
	{
		printf("infile : %d outfile %d\n", cmd_list->infile, cmd_list->outfile);
		printf("num_of_cmd : %d\n",cmd_list->num_of_cmd);
		if (cmd_list->cmd != NULL)
		{
			for (int x = 0; cmd_list->cmd[x]; x++)
			{
				printf("%s \n", cmd_list->cmd[x]);
			}
		}
		printf("\n");
		cmd_list = cmd_list->next;
	}
}

void	*error_clear(t_table *table)
{
	table->exit_status = return_error("minishell: syntax error\n", 258);
	return (NULL);
}

void	free_h_fd(t_heredoc_fd **h_fd)
{
	t_heredoc_fd	*head;

	if (!*h_fd)
		return ;
	while (*h_fd)
	{
		head = (*h_fd)->next;
		(*h_fd)->fd = 0;
		free(*h_fd);
		*h_fd = head;
	}
}

t_command	*check_open_file(t_cmd_info **node, t_table *table)
{
	t_heredoc_fd	*h_fd;
	t_command		*cmd_list;

	h_fd = check_heredoc(node, table);
	cmd_list = check_in_out_file(node, h_fd, table);
	free_h_fd(&h_fd);
	make_command(cmd_list, node);
	return (cmd_list);
}

void	free_list(t_tmp	**list)
{
	t_tmp	*node;

	if (!*list)
		return ;
	while (*list)
	{
		node = (*list)->next;
		if ((*list))
		{
			free((*list)->data);
			free((*list));
		}
		*list = node;
	}
}

void	free_node(t_cmd_info ***list)
{
	t_cmd_info	**head;
	t_cmd_info	*tmp;
	t_cmd_info	*tmp2;
	int	i;

	i = 0;
	head = *list;
	if (!*list)
		return ;
	while (head[i])
	{
		tmp2 = head[i];
		while (head[i])
		{
			tmp = head[i]->next;
			head[i]->type = 0;
			free(head[i]->data);
			free(head[i]);
			head[i] = tmp;
		}
		head[i] = tmp2;
		i++;
	}
	free(head);
}

void	*syntax_error_split(t_table *table, char ***tmp)
{
	free_split(*tmp);
	return (error_clear(table));
}

void	*syntax_error__(t_table *table, char ***tmp, t_tmp	**list, t_cmd_info ***node)
{
	free_split(*tmp);
	free_list(list);
	free_node(node);
	return (error_clear(table));
}

void	set_table(t_table *table)
{
	table->cmd_count = 0;
	table->fd_status = 0;
	table->syntax_error = 0;
}

t_command	*parse(char *command_line, t_table *table)
{
	char			**tmp1;
	t_tmp			*list;
	t_cmd_info		**node;
	t_command		*cmd_list;

	set_table(table);
	tmp1 = ft_split_quote_re(command_line, " ", table);
	if (table->syntax_error)
		return (syntax_error_split(table, &tmp1));
	list = make_cmd_list(tmp1, table);
	node = syntax_interpretation(list, table);
	check_syntax_error(node, table);
	if (table->syntax_error)
		return (syntax_error__(table, &tmp1, &list, &node));
	replace_argv_to_command(node);
	replace_environment_variable(node, table);
	if (table->syntax_error)
		return (syntax_error__(table, &tmp1, &list, &node));
	remove_dquote(node, table);
	cmd_list = check_open_file(node, table);
	//print_cmd(node);
	//print_cmd2(cmd_list);
	free_split(tmp1);
	free_list(&list);
	free_node(&node);
	if (!table->syntax_error && !table->fd_status)
	{
		//printf("syntax %d, fd_status %d\n", table->syntax_error, table->fd_status);
		table->exit_status = 0;
	}
	return (cmd_list);
}


void	free_command(t_command **lst)
{
	t_command	*node;

	while ((*lst))
	{
		node = (*lst)->next;
		free_split((*lst)->cmd);
		(*lst)->infile = 0;
		(*lst)->outfile = 0;
		free(*lst);
		*lst = node;
	}
}

void	free_env(t_table *table)
{
	char	**env;
	int		i;

	env = table->envp;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	handler_int(int signal)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG);
	if (signal == SIGINT)
	{
		if (pid == -1)
		{
			rl_on_new_line();
			rl_redisplay();	
			ft_putstr_fd("  \n", STDOUT_FILENO);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
		else
			ft_putstr_fd("\n", STDOUT_FILENO);
	}
	
}

void	handler_quit(int signal)
{
	pid_t	pid;
	int		status;

	pid = waitpid(-1, &status, WNOHANG);
	if(signal == SIGQUIT)
	{
		if (pid == -1) //ok.
		{
			rl_on_new_line();//입력 받은 것 종료
			rl_redisplay();// 입력받은 것 다시 출력
			ft_putstr_fd("  \b\b", STDOUT_FILENO);
		}
		else
			ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);//다시출력해서 커서가 글자의 끝에 있음.
	}
}

void	set_signal()
{
	signal(SIGINT, handler_int);
	signal(SIGQUIT, handler_quit);
}

int	command_size(t_command *command)
{
	int	i;

	i = 0;
	while (command)
	{
		i++;
		command = command->next;
	}
	return (i);
}

void	ft_exit(t_command *command, t_table *table)
{
	unsigned char ret;

	int	i;
	int	j;
	int	size;

	i = 0;
	if (!command || !command->cmd)
		return ;
	if (!ft_strcmp(command->cmd[0], "exit"))
	{
		size = command_size(command);
		if (size == 1)
			ft_putstr_fd("eixt\n", 1);
		j = 0;
		while (command->cmd[j])
			j++;
		if (j == 1)
		{
			table->exit_status = 0;
			exit(0);
			return ;
		}
		while (command->cmd[1][i + 1])
		{
			if (!(command->cmd[1][i] == '-' && ft_isdigit(command->cmd[1][i + 1]))\
			&& !ft_isdigit(command->cmd[1][i]))
			{
				printf("numeric argument required\n");
				table->exit_status = 255;
				exit(255);
				return ;
			}
			i++;
		}
		if (j == 3)
		{
			printf("tomany argument\n");
			table->exit_status = 1;
		}
		else if (j == 2)
		{
			ret = ft_atoi(command->cmd[1]);
			table->exit_status = ret;
			exit(ret);
		}
	}
}

int	main(int ac, char *av[], char *env[])
{
	t_table		table;
	t_command	*command;
	char		*input_command;

	table.envp = copy_env(env);
	set_signal();
	table.exit_status = 0;  // ---> 환경변수를 복사해둠으로써 팀원도 편하게 자원을 사용할 수 있도록 유도합니다
	while (ac && av)
	{
		input_command = readline("minishell$ "); // ---> 명령어 입력을 받도록합니다.
		if (!input_command)
			break ;
		if (*input_command != '\0' && !check_whitespace(input_command))
		{
			add_history(input_command); // 명령어를 기록하는 과정을 거칩니다 ----> history에 쌓이는 순사가 있을까요??
			command = parse(input_command, &table);
			ft_exit(command, &table);
			execute(&command, table.envp);
			free_command(&command);
		}
		free(input_command);
	}
	free_env(&table);
	ft_putstr_fd("\033[A", STDOUT_FILENO);
	ft_putstr_fd("\033[11C", STDOUT_FILENO);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (0);
}

// >> , > 무조건 뒤에 인자가 와야함
// 명령어는 더블 쿼터로 감쌀 수 있다
// 파이프 리다이렉션 다이렉션은 문자열로 감싸질 수 없다.
// 명령어 뒤에 파이프만 온 경우에 쉘 입력모드로 전환된다.
// 리다이렉션 뒤에는 무조건 파일이름이 와야합니다 파이프 리다이렉션 다이렉션은 올 수 없습니다