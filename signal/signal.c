/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:09:45 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 23:40:05 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handler_int(int signal)
{
	pid_t	pid;
	int		stat;

	pid = waitpid(-1, &stat, WNOHANG);
	if (signal == SIGINT)
	{
		if (g_sig == 1)
		{
			ft_putstr_fd("\n", STDOUT_FILENO);
			close(STDIN_FILENO);
			g_sig = 2;
		}
		else if (pid == -1)
		{
			ft_putstr_fd("\n", STDOUT_FILENO);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}	
}

void	set_signal(void)
{
	signal(SIGINT, handler_int);
	signal(SIGQUIT, SIG_IGN);
}

void	get_original_signal(void)
{
	signal(SIGQUIT, SIG_DFL);
}
