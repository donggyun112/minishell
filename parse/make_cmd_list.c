/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_cmd_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:38:47 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 21:54:55 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	push_tmp(t_tmp **list, char *data)
{
	t_tmp	*tmp;
	t_tmp	*head;

	tmp = (t_tmp *)malloc(sizeof(t_tmp));
	tmp->data = data;
	tmp->next = NULL;
	if (*list == NULL)
		*list = tmp;
	else
	{
		head = *list;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

t_tmp	*split_divid_operator(char **tmp1, t_table *table)
{
	int			i;
	int			j;
	char		**tmp2;
	t_tmp		*list;

	i = -1;
	list = NULL;
	if (!tmp1)
		return (NULL);
	while (tmp1[++i])
	{
		tmp2 = ft_split_operator(tmp1[i], "");
		if (!tmp2 || table->syntax_error)
			return (free_split(tmp2), free_split(&tmp1[i]));
		j = -1;
		while (tmp2[++j])
			push_tmp(&list, ft_strdup(tmp2[j]));
		free_split(tmp2);
	}
	return (list);
}
