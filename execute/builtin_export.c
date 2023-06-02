/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:52:30 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:53:09 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_export_error(char *str, int *ret)
{
	*ret = 1;
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
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
