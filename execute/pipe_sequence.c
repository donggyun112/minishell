/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_sequence.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:03:35 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 16:04:20 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_pipe(pid_t pid, t_command *tmp, int fd[])
{
	if (pid == 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		if (tmp->outfile == 1)
			dup2(fd[1], STDOUT_FILENO);
		else
		{
			dup2(tmp->outfile, STDOUT_FILENO);
			close(fd[1]);
		}
		close(fd[0]);
	}
	else
	{
		if (tmp->infile != 0)
			close(tmp->infile);
		close(fd[1]);
		if (tmp->next->infile == 0)
			tmp->next->infile = fd[0];
	}
}

void	set_pipe_last(pid_t pid, t_command *tmp, int fd[])
{
	if (pid == 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		dup2(tmp->outfile, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	set_pipe_builtin_last(pid_t pid, t_command *tmp, int fd[])
{
	if (pid > 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		dup2(tmp->outfile, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (tmp->outfile != STDOUT_FILENO)
			close(tmp->outfile);
		close(tmp->infile);
	}
	else
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	pipe_sequence(pid_t pid, t_command *tmp, t_fd *fds)
{
	if (tmp->next != NULL)
		set_pipe(pid, tmp, fds->fd);
	else if (tmp->next == NULL && check_builtin(tmp) && tmp->num_of_cmd == 0)
		set_pipe_builtin_last(pid, tmp, fds->fd);
	else if (tmp->next == NULL)
		set_pipe_last(pid, tmp, fds->fd);
}
