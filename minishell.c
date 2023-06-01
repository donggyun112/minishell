/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:01:22 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/02 00:29:56 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	end_set(void)
{
	ft_putstr_fd("\033[A", STDOUT_FILENO);
	ft_putstr_fd("\033[11C", STDOUT_FILENO);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
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
	list = split_divid_operator(tmp1, table);
	node = syntax_interpretation(list, table);
	check_syntax_error(node, table);
	if (table->syntax_error || !node)
		return (syntax_error__(table, &tmp1, &list, &node));
	replace_argv_to_command(node);
	remove_dquote(node, table);
	if (table->syntax_error)
		return (syntax_error__(table, &tmp1, &list, &node));
	cmd_list = check_open_file(node, table);
	free_all(&tmp1, &list, &node);
	if (!table->syntax_error && !table->fd_status)
		table->exit_status = 0;
	return (cmd_list);
}

int	main(int ac, char *av[], char *env[])
{
	t_table		table;
	t_command	*command;
	char		*input_command;

	init_env_and_exit_status(&table, env);
	while (ac && av)
	{
		table.save_fd = dup(STDIN_FILENO);
		input_command = readline("minishell$ ");
		if (!input_command)
			break ;
		if (*input_command != '\0' && !check_whitespace(input_command))
		{
			add_history(input_command);
			command = parse(input_command, &table);
			execute(&command, &table);
			free_command(&command);
		}
		check_sig(table.save_fd, &table);
		free(input_command);
		set_terminal();
	}
	free_env(&table);
	end_set();
	return (0);
}
