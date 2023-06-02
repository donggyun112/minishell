/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 19:54:11 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/02 17:00:02 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	re_place_get(t_remove_vaild	*d, t_table *table, t_cmd_info *tmp2, int i)
{
	(*d).tmp3 = replace_val(tmp2, table, (*d).flag);
	if ((*d).tmp3)
	{
		free((*d).tmp[i]);
		(*d).tmp[i] = (*d).tmp3;
		(*d).flag = 0;
	}
}

void	remove_quote_or_dquote(t_remove_vaild *d, t_cmd_info **node, int i)
{
	if (get_cmd_type(d->tmp[i]) == quote)
		d->flag = 1;
	else
		d->flag = 2;
	if ((*node)->type == redict_in)
		(*node)->heredoc_flag = special_heredoc;
	d->ret = ft_substr(d->tmp[i], 1, ft_strlen(d->tmp[i]) - 2);
	free(d->tmp[i]);
	d->tmp[i] = d->ret;
	d->flag3 = 1;
}
