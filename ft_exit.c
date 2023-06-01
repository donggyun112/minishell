/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:08:41 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 08:39:55 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	command_size(t_command *command)
{
	int	i;

	i = 0;
	while (command)
	{
		i++;
		command = command->next;
	}
	return (i);
}

void	exit_status(pid_t pid, t_table *table, int j, t_command *command)
{
	unsigned char	ret;

	if (j >= 3)
	{
		table->exit_status = 1;
		exit_error("too many arguments\n");
		if (pid == 0)
			exit(1);
	}
	else if (j == 2)
	{
		ret = ft_atoi(command->cmd[1]);
		exit(ret);
	}
}

void	ft_exit(pid_t pid, t_command *command, t_table *table)
{
	int	i;
	int	j;
	int	size;

	i = 0;
	size = command_size(command);
	if (size == 1 && pid != 0)
		ft_putstr_fd("exit\n", 1);
	j = 0;
	while (command->cmd[j])
		j++;
	if (j == 1)
		exit(0);
	while (command->cmd[1][i + 1])
	{
		if (!(command->cmd[1][i] == '-' && ft_isdigit(command->cmd[1][i + 1])) \
		&& !ft_isdigit(command->cmd[1][i]))
		{
			exit_error("numeric argument required\n");
			exit(255);
		}
		i++;
	}
	exit_status(pid, table, j, command);
}
