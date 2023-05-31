/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 23:47:39 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 02:26:34 by dongkseo         ###   ########.fr       */
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

int	is_single(char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		if (i > 1 && tmp[i] == '$' && tmp[i - 2] == '\"'\
		&& tmp[i - 1] == '\"' && tmp[i + 1] == '\"')
			return (1);
		i++;
	}
	return (0);
}
