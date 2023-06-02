/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:42:24 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:42:59 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
