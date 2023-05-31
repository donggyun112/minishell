/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 04:45:39 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 23:43:39 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_sig(int fd, t_table *table)
{
	if (g_sig)
	{
		dup2(fd, STDIN_FILENO);
		table->exit_status = 1;
		g_sig = 0;
	}
	close(fd);
}

void	init_env_and_exit_status(t_table *table, char **env)
{
	table->exit_status = 0;
	table->envp = copy_env(env);
	set_terminal();
	set_signal();
}

void	set_table(t_table *table)
{
	table->cmd_count = 0;
	table->fd_status = 0;
	table->syntax_error = 0;
	table->builtin_exit = 0;
}

t_cmd_info	**set_cmd_list(t_table *table, t_tmp *list)
{
	int			i;
	t_cmd_info	**tmp;

	table->cmd_count = cmd_size(list);
	tmp = (t_cmd_info **)malloc(sizeof(t_cmd_info *) * (table->cmd_count + 1));
	tmp[table->cmd_count] = NULL;
	i = -1;
	while (++i < table->cmd_count)
		tmp[i] = NULL;
	return (tmp);
}
