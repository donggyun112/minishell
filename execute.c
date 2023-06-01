/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 18:23:00 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/01 17:28:28 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

void	exit_processor_error(t_command **cmd)
{
	free_command(cmd);
	exit (1);
}

void	exit_access_error(char *file, int err_stat)
{
	char	*err_str;

	if (err_stat == PATH)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		exit(1);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		err_str = ft_strjoin(file, ": command not found");
		ft_putendl_fd(err_str, 2);
		exit(1);
	}
}

char	*ft_getenv(char *name, char **envp)
{
	char	*rename;
	char	*env;
	int		idx;
	int		j;

	rename = ft_strjoin(name, "=");
	env = NULL;
	idx = 0;
	j = 0;
	while (envp[idx] != NULL)
	{
		if (ft_strncmp(envp[idx], rename, ft_strlen(rename)) == 0)
		{
			while (envp[idx][j] && envp[idx][j] != '=')
				j++;
			env = &envp[idx][j + 1];
			break ;
		}
		idx++;
	}
	free(rename);
	return (env);
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

void	free_strarr(char **path)
{
	int	idx;

	idx = 0;
	while (path[idx] != NULL)
		free(path[idx++]);
	free(path);
}

void	exit_no_exec_file(char *path)
{
	if (ft_strcmp(path, ".") == 0)
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		exit(1);
	}
	else if (ft_strcmp(path, "..") == 0)
	{
		ft_putendl_fd("minishell: ..: command not found", 2);
		exit(1);
	}
	else if (ft_strcmp(path, "/") == 0)
	{
		ft_putendl_fd("minishell: /: is a directory", 2);
		exit(1);
	}
	else
	{
		ft_putendl_fd("minishell: : command not found", 2);
		exit(1);
	}
}

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

int	echo_option_n(char *option)
{
	int	idx;

	idx = 0;
	if (option[idx] != '-')
		return (0);
	else
		idx++;
	while (option[idx])
	{
		if (option[idx] != 'n')
			return (0);
		idx++;
	}
	return (1);
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
	while (cmd[idx] && echo_option_n(cmd[idx]))
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
	char	*rename;
	int		idx;
	int		flag;

	idx = 0;
	flag = 0;
	rename = ft_strjoin(name, "=");
	while (envp[idx] != NULL)
	{
		if (ft_strncmp(rename, envp[idx], (size_t)ft_strlen(rename)) == 0)
		{
			flag = 1;
			break ;
		}
		idx++;
	}
	if (flag == 1)
	{
		tmp = ft_strjoin(rename, value);
		free(envp[idx]);
		envp[idx] = NULL;
		envp[idx] = tmp;
	}
	free(rename);
}

char	*find_path(char *cur, char *cmd, char **envp)
{
	if (cmd == NULL)
		return (ft_strdup(ft_getenv("HOME", envp)));
	if (cmd[0] == '~')
		return (ft_strjoin(ft_getenv("HOME", envp), &cmd[1]));
	else if (cmd[0] == '/')
		return (ft_strdup(cmd));
	else
		return (link_path(cur, cmd));
	return (NULL);
}

int	print_cd_error(char *err_str, char **cur, char **pwd, char **new_path)
{
	ft_putstr_fd("minishell: ", 2);
	if (err_str[0] == '~' && err_str[1] == '/')
		perror(*new_path);
	else
		perror(err_str);
	free(*cur);
	free(*pwd);
	free(*new_path);
	return (1);
}

int	builtin_cd(t_command *tmp, char ***envp_addr)
{
	char	*cur;
	char	*pwd;
	char	*new_path;
	char	**envp;

	cur = (char *)malloc(1024);
	pwd = (char *)malloc(1024);
	envp = *envp_addr;
	getcwd(cur, 1024);
	new_path = find_path(cur, tmp->cmd[1], envp);
	if (access(new_path, F_OK) != 0)
		return (print_cd_error(tmp->cmd[1], &cur, &pwd, &new_path));
	if (chdir(new_path) != 0)
		return (print_cd_error(tmp->cmd[1], &cur, &pwd, &new_path));
	mod_envp("OLDPWD", cur, envp);
	getcwd(pwd, 1024);
	mod_envp("PWD", pwd, envp);
	free(cur);
	free(pwd);
	free(new_path);
	return (0);
}

int	builtin_env(char ***envp_addr)
{
	int		idx;
	char	**envp;

	idx = 0;
	envp = *envp_addr;
	while (envp[idx] != NULL)
	{
		if (ft_strchr(envp[idx], 61))
		{
			write(STDOUT_FILENO, envp[idx], ft_strlen(envp[idx]));
			write(STDOUT_FILENO, "\n", 1);
		}
		idx++;
	}
	return (0);
}

int	builtin_pwd(void)
{
	char	*pwd;

	pwd = (char *)malloc(1024);
	getcwd(pwd, 1024);
	if (pwd == NULL)
		return (1);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}

int	check_unset_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] == '_' || ((str[i] >= 'a' && str[i] <= 'z')
					|| (str[i] >= 'A' && str[i] <= 'Z'))))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(str, 2);
			ft_putendl_fd("': not a valid identifier", 2);
			return (1);
		}
		i++;
	}
	return (0);
}

int	remove_exist_env(char **envp, int n_env, int idx, char **cmd)
{
	if (envp[n_env][ft_strlen(cmd[idx])] == '\0'
					|| envp[n_env][ft_strlen(cmd[idx])] == '=')
	{
		free(envp[n_env]);
		envp[n_env] = ft_strdup("");
		return (1);
	}
	return (0);
}

int	remove_env(char **cmd, char **envp)
{
	int		idx;
	int		n_env;
	int		ret;

	idx = 1;
	ret = 0;
	while (cmd[idx] != NULL)
	{
		n_env = 0;
		ret = ret + check_unset_valid(cmd[idx]);
		while (envp[n_env] != NULL)
		{
			if (ft_strncmp(envp[n_env], cmd[idx], ft_strlen(cmd[idx])) == 0)
				if (remove_exist_env(envp, n_env, idx, cmd))
					break ;
			n_env++;
		}
		idx++;
	}
	if (ret > 0)
		return (1);
	else
		return (0);
}

int	get_env_size(char **envp)
{
	int	idx;
	int	size;

	idx = 0;
	size = 0;
	while (envp[idx])
	{
		if (envp[idx][0] != '\0')
			size++;
		idx++;
	}
	return (size);
}

char	**reset_env(char **env_copy)
{
	int		size;
	int		idx;
	char	**new_env;

	idx = 0;
	size = get_env_size(env_copy);
	new_env = (char **)malloc(sizeof(char *) * (size + 1));
	if (new_env == NULL)
		return (NULL);
	new_env[size] = NULL;
	size = 0;
	while (env_copy[idx] != NULL)
	{
		if (env_copy[idx][0] != '\0')
		{
			new_env[size] = ft_strdup(env_copy[idx]);
			size++;
		}
		idx++;
	}
	return (new_env);
}

int	builtin_unset(char **cmd, char ***envp_addr)
{
	char	**env_copy;
	char	**new_envp;
	char	**envp;
	int		ret;

	envp = *envp_addr;
	env_copy = copy_env(envp);
	if (env_copy == NULL)
		return (1);
	ret = remove_env(cmd, env_copy);
	new_envp = reset_env(env_copy);
	if (new_envp == NULL)
	{
		free_strarr(env_copy);
		return (1);
	}
	free_strarr(env_copy);
	free_strarr(envp);
	*envp_addr = new_envp;
	return (ret);
}

void	print_sorted_ascii(char **envp)
{
	int	idx;
	int	cursor;

	idx = 0;
	while (envp[idx] != NULL)
	{
		cursor = 0;
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		while (envp[idx][cursor] != '\0' && envp[idx][cursor] != '=')
		{
			write (1, &envp[idx][cursor], 1);
			cursor++;
		}
		if (envp[idx][cursor] == '=')
		{
			write(1, "=\"", 2);
			ft_putstr_fd(&envp[idx][cursor + 1], 1);
			write(1, "\"", 1);
		}
		write (1, "\n", 1);
		idx++;
	}
}

int	print_export(char **envp)
{
	int		i;
	int		j;
	char	*tmp;
	char	**envp_copy;

	i = 0;
	envp_copy = copy_env(envp);
	while (envp_copy[i])
	{
		j = i + 1;
		while (envp_copy[j])
		{
			if (ft_strcmp(envp_copy[i], envp_copy[j]) > 0)
			{
				tmp = envp_copy[i];
				envp_copy[i] = envp_copy[j];
				envp_copy[j] = tmp;
			}
			j++;
		}
		i++;
	}
	print_sorted_ascii(envp_copy);
	free_strarr(envp_copy);
	return (0);
}

int	print_export_error(char *str, int *ret)
{
	*ret = 1;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

void	export_add(char ***envp_addr, char *cmd)
{
	int		size;
	int		idx;
	char	**old_envp;
	char	**new_envp;

	old_envp = *envp_addr;
	size = 0;
	idx = 0;
	while (old_envp[size])
		size++;
	new_envp = (char **)malloc(sizeof(char *) * (size + 2));
	new_envp[size + 1] = NULL;
	while (old_envp[idx] != NULL)
	{
		new_envp[idx] = ft_strdup(old_envp[idx]);
		idx++;
	}
	new_envp[idx] = ft_strdup(cmd);
	free_strarr(old_envp);
	*envp_addr = new_envp;
}

void	add_value_exist(char *name, char *cmd, char ***envp_addr)
{
	char	**envp;
	int		idx;
	int		flag;

	idx = 0;
	flag = 0;
	envp = *envp_addr;
	while (envp[idx])
	{
		if (ft_strncmp(envp[idx], name, ft_strlen(name)) == 0)
		{
			if (envp[idx][ft_strlen(name)] == '\0'
				|| envp[idx][ft_strlen(name)] == '=')
			{
				free(envp[idx]);
				envp[idx] = ft_strdup(cmd);
				flag = 1;
				break ;
			}
		}
		idx++;
	}
	if (flag == 0)
		export_add(envp_addr, cmd);
}

void	add_no_value(char *cmd, char ***envp_addr)
{
	int		idx;
	char	**envp;
	int		flag;

	idx = 0;
	flag = 0;
	envp = *envp_addr;
	while (envp[idx])
	{
		if (ft_strncmp(envp[idx], cmd, ft_strlen(cmd)) == 0)
		{
			if (envp[idx][ft_strlen(cmd)] == '\0'
				|| envp[idx][ft_strlen(cmd)] == '=')
				flag = 1;
		}
		idx++;
	}
	if (flag == 0)
		export_add(envp_addr, cmd);
}

void	add_env_list(char *cmd, char end, char ***envp_addr)
{
	char	*rename;
	int		i;
	int		j;
	int		pos;

	i = 0;
	j = 0;
	if (end != '\0' && end != '=')
		return ;
	rename = ft_strchr(cmd, '=');
	if (rename)
	{
		pos = rename - cmd;
		rename = ft_substr(cmd, 0, pos);
		add_value_exist(rename, cmd, envp_addr);
		free(rename);
	}
	else
		add_no_value(cmd, envp_addr);
}

int	check_export_valid(char **cmd, char ***envp_addr)
{
	int	idx;
	int	cursor;
	int	ret;

	idx = 0;
	ret = 0;
	while (cmd[++idx])
	{
		cursor = -1;
		if (cmd[idx][0] == '=')
		{
			print_export_error(cmd[idx], &ret);
			continue ;
		}
		while (cmd[idx][++cursor] && cmd[idx][cursor] != '=')
		{
			if (!(cmd[idx][cursor] == '_'
				|| ((cmd[idx][cursor] >= 'a' && cmd[idx][cursor] <= 'z')
					|| (cmd[idx][cursor] >= 'A' && cmd[idx][cursor] <= 'Z'))))
				if (print_export_error(cmd[idx], &ret))
					break ;
		}
		add_env_list(cmd[idx], cmd[idx][cursor], envp_addr);
	}
	return (ret);
}

int	builtin_export(t_command *tmp, char ***envp_addr)
{
	char	**envp;
	char	**cmd;
	int		ret;

	envp = *envp_addr;
	cmd = tmp->cmd;
	if (cmd[1] == NULL)
		return (print_export(envp));
	else
	{
		ret = check_export_valid(tmp->cmd, envp_addr);
		return (ret);
	}
}

int	run_builtin(pid_t pid, t_command *tmp, t_table *table)
{
	if (tmp->infile < 0 || tmp->outfile < 0)
		return (127);
	if (ft_strcmp(tmp->cmd[0], "echo") == 0)
		return (builtin_echo(tmp));
	else if (ft_strcmp(tmp->cmd[0], "cd") == 0)
		return (builtin_cd(tmp, &(table->envp)));
	else if (ft_strcmp(tmp->cmd[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(tmp->cmd[0], "export") == 0)
		return (builtin_export(tmp, &(table->envp)));
	else if (ft_strcmp(tmp->cmd[0], "unset") == 0)
		return (builtin_unset(tmp->cmd, &(table->envp)));
	else if (ft_strcmp(tmp->cmd[0], "env") == 0)
		return (builtin_env(&(table->envp)));
	else if (ft_strcmp(tmp->cmd[0], "exit") == 0)
	{
		ft_exit(pid, tmp, table);
		return (table->exit_status);
	}
	else
		return (1);
}

int	check_builtin(t_command *tmp)
{
	char	*cmd;

	if (tmp->cmd == NULL)
		return (0);
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
			exit(1);
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
	set_terminal();
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(tmp->cmd[0]);
	exit(1);
}

void	exec_parent(pid_t pid, t_fd *fds, t_command *tmp, t_table *table)
{
	char	***envp;

	envp = &(table->envp);
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
