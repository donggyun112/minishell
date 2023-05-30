/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 04:45:39 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 04:45:58 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
}