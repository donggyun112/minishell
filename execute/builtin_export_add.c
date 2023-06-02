/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_add.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:49:27 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:56:30 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	int		pos;

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
