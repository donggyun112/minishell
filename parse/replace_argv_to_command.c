/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_argv_to_command.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:48:01 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 23:47:56 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	replace_argv_to_command(t_cmd_info **cmd)
{
	int			i;
	t_cmd_info	*head;

	i = 0;
	if (!cmd)
		return ;
	while (cmd[i])
	{
		head = cmd[i];
		while (cmd[i])
		{
			if (cmd[i]->type == argv)
			{
				cmd[i]->type = command;
				break ;
			}
			cmd[i] = cmd[i]->next;
		}
		cmd[i] = head;
		i++;
	}
}
