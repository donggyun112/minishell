/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:27:22 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:58:35 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	reset_fd(t_fd *fds)
{
	dup2(fds->std_fd[0], STDIN_FILENO);
	dup2(fds->std_fd[1], STDOUT_FILENO);
}
