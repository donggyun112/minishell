/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:38:59 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:55:38 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
