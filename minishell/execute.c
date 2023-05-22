/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 18:23:00 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/05/22 20:12:46 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_processor_error(t_command **cmd)
{
	free_command(cmd);
	exit (1);
}

void	exit_exec_error(char *file)
{
	perror(file);
	exit(1);
}

char	*link_path(char *path, char *cmd)
{
	char	*tmp1;
	char	*tmp2;

	tmp1 = ft_strjoin(path, "/");
	tmp2 = ft_strjoin(tmp1, cmd);
	free(tmp1);
	return (tmp2);
}

void	set_exec_path(char **envp, char **cmd)
{
	char	**path;
	char	*tmp;
	int		idx;

	if (cmd[0][0] == '/' || (cmd[0][0] == '.' && cmd[0][1] == '/'))
	{
		if (access(cmd[0], X_OK) == 0)
			return ;
		exit_exec_error(cmd[0]);
	}
	idx = 0;
	path = ft_split(getenv("PATH"), ':');
	while (path[idx] != NULL)
	{
		tmp = link_path(path[idx], cmd[0]);
		if (access(tmp, X_OK) == 0)
		{
			free(cmd[0]);
			cmd[0] = tmp;
			return ;
		}
		free(tmp);
		idx++;
	}
	exit_exec_error(cmd[0]);
}

void	builtin_echo(t_command *tmp)
{
	char	**cmd;
	int		idx;
	int		opt;
	int		size;

	idx = 1;
	opt = 0;
	cmd = tmp->cmd;
	if (cmd[1] != NULL && ft_strcmp(cmd[1], "-n") == 0)
	{
		idx++;
		opt = 1;
	}
	while (cmd[idx] != NULL)
	{
		size = ft_strlen(cmd[idx]);
		if (write(STDOUT_FILENO, cmd[idx], size) != size)
			return ;
		if (cmd[idx + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		idx++;
	}
	if (opt == 0)
		write(STDOUT_FILENO, "\n", 1);
}

void	mod_envp(char *name, char *value, char **envp)
{
	char	*tmp;
	int		idx;
	int		flag;

	idx = 0;
	flag = 0;
	while (envp[idx] != NULL)
	{
		if (ft_strncmp(name, envp[idx++], (size_t)ft_strlen(name)) == 0)
		{
			flag = 1;
			break ;
		}
	}
	if (flag == 1)
	{
		tmp = ft_strjoin(name, value);
		free(envp[idx]);
		envp[idx] = tmp;
	}
}

char	*find_path(char *cur, char *cmd)
{
	if (cmd[0] != '.' && cmd[0] != '/')
		return (link_path(cur, cmd));
	return (NULL);
}

void	builtin_cd(t_command *tmp, char **envp)
{
	char	*cur;
	char	*new_path;

	cur = (char *)malloc(1000);
	(void)envp;
	getcwd(cur, 1000);
	new_path = find_path(cur, tmp->cmd[1]);
	if (access(new_path, F_OK) != 0)
	{
		perror(tmp->cmd[1]);
		return ;
	}
	chdir(new_path);
	mod_envp("OLDPWD=", cur, envp);
	mod_envp("PWD=", new_path, envp);
	free(cur);
	free(new_path);
}

void	builtin_env(char **envp)
{
	int	idx;

	idx = 0;
	while (envp[idx] != NULL)
	{
		ft_putendl_fd(envp[idx], STDOUT_FILENO);
		idx++;
	}
}

void	run_builtin(t_command *tmp, char **envp)
{
	if (ft_strcmp(tmp->cmd[0], "echo") == 0)
		builtin_echo(tmp);
	else if (ft_strcmp(tmp->cmd[0], "cd") == 0)
		builtin_cd(tmp, envp);
	// else if (ft_strcmp(tmp->cmd[0], "pwd") == 0)
	// 	builtin_pwd();
	// else if (ft_strcmp(tmp->cmd[0], "export") == 0)
	// 	builtin_export();
	// else if (ft_strcmp(tmp->cmd[0], "unset") == 0)
	// 	builtin_unset();
	else if (ft_strcmp(tmp->cmd[0], "env") == 0)
		builtin_env(envp);
	// else if (ft_strcmp(tmp->cmd[0], "exit") == 0)
	// 	builtin_exit();
	else
		return ;
}

int	check_builtin(t_command *tmp)
{
	char	*cmd;

	cmd = tmp->cmd[0];
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (0);
	else
		return (1);
}

void	reset_fd(int fd[])
{
	dup2(STDIN_FILENO, fd[0]);
	dup2(STDOUT_FILENO, fd[1]);
}

void	exec_cmd(pid_t pid, int fd[], t_command *tmp, char **envp)
{
	if (pid == 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		if (tmp->next != NULL)
			dup2(fd[1], STDOUT_FILENO);
		else
			dup2(tmp->outfile, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		if (check_builtin(tmp) == 0)
			exit(0);
		set_exec_path(envp, tmp->cmd);
		execve(tmp->cmd[0], tmp->cmd, envp);
	}
	else
	{
		if (tmp->infile != 0)
			close(tmp->infile);
		if (tmp->next != NULL && tmp->next->infile == 0)
			tmp->next->infile = fd[0];
		close(fd[1]);
		if (check_builtin(tmp) == 0)
		{
			run_builtin(tmp, envp);
			reset_fd(fd);
		}
	}
}

void	set_pipe(pid_t pid, t_command *tmp, int fd[])
{
	if (pid == 0)
	{
		dup2(tmp->infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		// close(fd[1]);
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

void	set_pipe_builtin(pid_t pid, t_command *tmp, int fd[])
{
	if (pid > 0)
	{
		dup2(tmp->infile, STDIN_FILENO); //reset after run builtin!
		dup2(fd[1], STDOUT_FILENO); //reset after run builtin!
		close(fd[0]);
		if (tmp->infile != 0)
			close(tmp->infile);
		if (tmp->next->infile == 0)
			tmp->next->infile = fd[0];
	}
	else
	{
		close(fd[1]);
	}
}

void	set_pipe_builtin_last(pid_t pid, t_command *tmp, int fd[])
{
	if (pid > 0)
	{
		dup2(tmp->infile, STDIN_FILENO); //reset after run builtin!
		dup2(tmp->outfile, STDOUT_FILENO); //reset after run builtin!
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

void	pipe_sequence(pid_t pid, t_command *tmp, int fd[])
{
	if (tmp->next != NULL && check_builtin(tmp))
		set_pipe(pid, tmp, fd);
	else if (tmp->next == NULL && check_builtin(tmp))
		set_pipe_last(pid, tmp, fd);
	else if (tmp->next != NULL && check_builtin(tmp) == 0)
		set_pipe_builtin(pid, tmp, fd);
	else if (tmp->next == NULL && check_builtin(tmp) == 0)
		set_pipe_builtin_last(pid, tmp, fd);
}

void	execute(t_command **cmd, char **envp)
{
	pid_t		pid;
	t_command	*tmp;
	int			fd[2];
	int			std_backup[2];
	int			status;

	tmp = *cmd;
	std_backup[0] = dup(STDIN_FILENO);
	std_backup[0] = dup(STDIN_FILENO);
	while (tmp != NULL)
	{
		if (pipe(fd) < 0)
			exit_processor_error(cmd);
		pid = fork();
		if (pid < 0)
			exit_processor_error(cmd);
		pipe_sequence(pid, tmp, fd);
		exec_cmd(pid, fd, tmp, envp);
		tmp = tmp->next;
	}
	// dup2(0, std_backup[0]);
	// dup2(1, std_backup[1]);
	while (waitpid(0, &status, 0) > 0)
		;
}
