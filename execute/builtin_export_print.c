/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:44:08 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:56:48 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
