/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:33:18 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:33:45 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
