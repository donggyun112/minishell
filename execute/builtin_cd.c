/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:36:21 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:36:37 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
