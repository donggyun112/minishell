/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 23:47:39 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 23:49:22 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_pipe(t_table *table, t_tmp *list)
{
	if (get_cmd_type(list->data) == pipe_)
	{
		table->syntax_error = 1;
		return (1);
	}
	while (list)
	{
		if (get_cmd_type(list->data) == pipe_)
		{
			if (!list->next || !ft_strcmp(list->data, "|"))
			{
				table->syntax_error = 1;
				return (1);
			}
		}
		list = list->next;
	}
	return (0);
}
