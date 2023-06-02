/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:00:18 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 16:02:48 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_dir(char *exec_path)
{
	struct stat	dict_stat;

	if (stat(exec_path, &dict_stat) == 0)
	{
		if ((dict_stat.st_mode & S_IFMT) == S_IFDIR)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(exec_path, 2);
			ft_putendl_fd(": is a directory", 2);
			exit(126);
		}
	}
}

void	exec_child(pid_t pid, t_command *tmp, t_table *table)
{
	char	*exec_path;
	char	***envp;

	envp = &(table->envp);
	if (tmp->next == NULL && tmp->num_of_cmd == 0 && check_builtin(tmp))
		exit(0);
	else if (check_builtin(tmp) == 1)
		exit(run_builtin(pid, tmp, table));
	if (tmp->infile < 0 || tmp->outfile < 0)
		exit(1);
	exec_path = set_exec_path(envp, tmp->cmd);
	is_dir(exec_path);
	get_original_signal();
	set_origterminal();
	execve(exec_path, tmp->cmd, *envp);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(tmp->cmd[0]);
	exit(1);
}

void	exec_parent(pid_t pid, t_fd *fds, t_command *tmp, t_table *table)
{
	if (tmp->infile > 0)
		close(tmp->infile);
	if (tmp->next != NULL && tmp->next->infile == 0)
		tmp->next->infile = fds->fd[0];
	close(fds->fd[1]);
	if (tmp->next == NULL && tmp->num_of_cmd == 0 && check_builtin(tmp))
	{
		table->builtin_exit = run_builtin(pid, tmp, table);
		reset_fd(fds);
	}
}

void	exec_cmd(pid_t pid, t_fd *fds, t_command *tmp, t_table *table)
{
	if (pid == 0)
	{
		if (tmp->infile >= 0 && tmp->outfile >= 1 && tmp->cmd == NULL)
			exit (0);
		else if (tmp->cmd == NULL)
			exit (1);
		exec_child(pid, tmp, table);
	}
	else
	{
		if (tmp->infile >= 0 && tmp->outfile >= 1 && tmp->cmd == NULL)
		{
			table->exit_status = 0;
			return ;
		}
		else if (tmp->cmd == NULL)
		{
			table->exit_status = 1;
			return ;
		}
		exec_parent(pid, fds, tmp, table);
	}
}
