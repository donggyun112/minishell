/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 18:23:00 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 16:05:37 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*set_abs_rel_path(char *path)
{
	if (ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0
		|| path[0] == '\0' || ft_strcmp(path, "/") == 0)
		exit_no_exec_file(path);
	if (access(path, F_OK) == 0)
		return (path);
	exit_access_error(path, PATH);
	return (NULL);
}

char	*set_exec_path(char ***envp_addr, char **cmd)
{
	char	**path;
	char	*tmp;
	char	**envp;
	int		idx;

	envp = *envp_addr;
	if (cmd[0][0] == '/' || cmd[0][0] == '.' || cmd[0][0] == '\0')
		return (set_abs_rel_path(cmd[0]));
	idx = 0;
	path = ft_split(ft_getenv("PATH", envp), ':');
	while (path != NULL && path[idx] != NULL)
	{
		tmp = link_path(path[idx], cmd[0]);
		if (access(tmp, X_OK) == 0)
		{
			free_strarr(path);
			return (tmp);
		}
		free(tmp);
		idx++;
	}
	exit_access_error(cmd[0], CMD);
	return (NULL);
}

void	set_exit_status(pid_t pid, t_table *table)
{
	int	exit_value;

	waitpid(pid, &exit_value, 0);
	if (exit_value == SIGINT)
		ft_putstr_fd("\n", STDERR_FILENO);
	else if (exit_value == SIGQUIT)
		ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
	if (exit_value == SIGINT)
		table->exit_status = 130;
	else if (exit_value == SIGQUIT)
		table->exit_status = 131;
	else
	{
		if (table->builtin_exit != 0)
		{
			table->exit_status = table->builtin_exit;
			table->builtin_exit = 0;
		}
		else
			table->exit_status = WEXITSTATUS(exit_value);
	}
}

void	execute(t_command **cmd, t_table *table)
{
	pid_t		pid;
	t_command	*tmp;
	t_fd		fds;
	int			status;

	if (table->syntax_error || g_sig == 2)
		return ;
	tmp = *cmd;
	fds.std_fd[0] = dup(STDIN_FILENO);
	fds.std_fd[1] = dup(STDOUT_FILENO);
	while (tmp != NULL)
	{
		if (pipe(fds.fd) < 0)
			exit_processor_error(cmd);
		pid = fork();
		if (pid < 0)
			exit_processor_error(cmd);
		pipe_sequence(pid, tmp, &fds);
		exec_cmd(pid, &fds, tmp, table);
		tmp = tmp->next;
	}
	set_exit_status(pid, table);
	while (waitpid(0, &status, 0) > 0)
		;
}
