/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 18:23:00 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/05/24 20:01:55 by jinhyeop         ###   ########.fr       */
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

int	builtin_echo(t_command *tmp)
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
			return (1);
		if (cmd[idx + 1] != NULL)
			write(STDOUT_FILENO, " ", 1);
		idx++;
	}
	if (opt == 0)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
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

int	builtin_cd(t_command *tmp, char **envp)
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
		return (1);
	}
	chdir(new_path);
	mod_envp("OLDPWD=", cur, envp);
	mod_envp("PWD=", new_path, envp);
	free(cur);
	free(new_path);
	return (0);
}

int	builtin_env(char **envp)
{
	int	idx;

	idx = 0;
	while (envp[idx] != NULL)
	{
		write(STDOUT_FILENO, envp[idx], ft_strlen(envp[idx]));
		write(STDOUT_FILENO, "\n", 1);
		idx++;
	}
	return (0);
}

int	run_builtin(t_command *tmp, char **envp)
{
	if (ft_strcmp(tmp->cmd[0], "echo") == 0)
		return (builtin_echo(tmp));
	else if (ft_strcmp(tmp->cmd[0], "cd") == 0)
		return (builtin_cd(tmp, envp));
	// else if (ft_strcmp(tmp->cmd[0], "pwd") == 0)
	// 	return (builtin_pwd());
	// else if (ft_strcmp(tmp->cmd[0], "export") == 0)
	// 	return (builtin_export());
	// else if (ft_strcmp(tmp->cmd[0], "unset") == 0)
	// 	return (builtin_unset());
	else if (ft_strcmp(tmp->cmd[0], "env") == 0)
		return (builtin_env(envp));
	// else if (ft_strcmp(tmp->cmd[0], "exit") == 0)
	// 	return (builtin_exit());
	else
		return (1);
}

int	check_builtin(t_command *tmp)
{
	char	*cmd;

	cmd = tmp->cmd[0];
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

void	reset_fd(t_fd *fds)
{
	dup2(fds->std_fd[0], STDIN_FILENO);
	dup2(fds->std_fd[1], STDOUT_FILENO);
}

void	exec_child(pid_t pid, t_fd *fds, t_command *tmp, char **envp)
{
	if (tmp->next == NULL && tmp->num_of_cmd == 0 && check_builtin(tmp))
		exit(0);
	else if (check_builtin(tmp) == 1)
		exit(run_builtin(tmp, envp));
	set_exec_path(envp, tmp->cmd);
	execve(tmp->cmd[0], tmp->cmd, envp);
	exit(-1);
}

void	exec_parent(pid_t pid, t_fd *fds, t_command *tmp, char **envp)
{
	if (tmp->infile != 0)
		close(tmp->infile);
	if (tmp->next != NULL && tmp->next->infile == 0)
		tmp->next->infile = fds->fd[0];
	close(fds->fd[1]);
	if (tmp->next == NULL && tmp->num_of_cmd == 0 && check_builtin(tmp))
	{
		run_builtin(tmp, envp);
		reset_fd(fds);
	}
}

void	exec_cmd(pid_t pid, t_fd *fds, t_command *tmp, char **envp)
{
	if (pid == 0)
	{
		if (tmp->cmd == NULL)
			exit (0);
		exec_child(pid, fds, tmp, envp);
	}
	else
	{
		if (tmp->cmd == NULL)
			return ;
		exec_parent(pid, fds, tmp, envp);
	}
}

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

// void	set_pipe_builtin(pid_t pid, t_command *tmp, int fd[])
// {
// 	if (pid > 0)
// 	{
// 		dup2(tmp->infile, STDIN_FILENO);
// 		if (tmp->outfile == 1)
// 			dup2(fd[1], STDOUT_FILENO);
// 		else
// 		{
// 			dup2(tmp->outfile, STDOUT_FILENO);
// 			close(fd[1]);
// 		}
// 		close(fd[0]);
// 		if (tmp->infile != 0)
// 			close(tmp->infile);
// 		// if (tmp->next->infile == 0)
// 		// 	tmp->next->infile = fd[0];
// 	}
// 	else
// 	{
// 		close(fd[0]);
// 		close(fd[1]);
// 	}
// }

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

void	pipe_sequence(pid_t pid, t_command *tmp, t_fd *fds)
{
	if (tmp->cmd == NULL && tmp->next == NULL)
		set_pipe_last(pid, tmp, fds->fd);
	else if (tmp->cmd == NULL && tmp->next != NULL)
		set_pipe(pid, tmp, fds->fd);
	else if (tmp->next != NULL)
		set_pipe(pid, tmp, fds->fd);
	else if (tmp->next == NULL && check_builtin(tmp) && tmp->num_of_cmd == 0)
		set_pipe_builtin_last(pid, tmp, fds->fd);
	else if (tmp->next == NULL)
		set_pipe_last(pid, tmp, fds->fd);
}	// if cmd == NULL, check_builtin modify

void	execute(t_command **cmd, char **envp)
{
	pid_t		pid;
	t_command	*tmp;
	t_fd		fds;
	int			status;

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
		exec_cmd(pid, &fds, tmp, envp);
		tmp = tmp->next;
	}
	while (waitpid(0, &status, 0) > 0)
		;
}

//builtin 파이프 연결 안되는거 고쳐야됨
//builtin 구현기능 아예 새로 하는게 빠를지도?
//builtin이 단독으로 있을때는 부모프로세스에서 실행되게 하고
//builtin이 다른 커맨드와 같이 있을때는 자식프로세스에서 실행되게 해야 bash의 동작과 같아질 수 있다...
