/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:05:51 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 22:57:51 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*free_split(char **base)
{
	int	i;

	i = -1;
	if (!base)
		return (NULL);
	while (base[++i])
		free(base[i]);
	free(base);
	return (NULL);
}

void	*free_return_null(t_replace *d)
{
	if (!d)
		return (NULL);
	free(d->ret);
	return (NULL);
}

void	free_h_fd(t_heredoc_fd **h_fd)
{
	t_heredoc_fd	*head;

	if (!*h_fd)
		return ;
	while (*h_fd)
	{
		head = (*h_fd)->next;
		(*h_fd)->fd = 0;
		free(*h_fd);
		*h_fd = head;
	}
}

void	free_list(t_tmp	**list)
{
	t_tmp	*node;

	if (!*list)
		return ;
	while (*list)
	{
		node = (*list)->next;
		if ((*list))
		{
			free((*list)->data);
			free((*list));
		}
		*list = node;
	}
}

void	free_node(t_cmd_info ***list)
{
	t_cmd_info	**head;
	t_cmd_info	*tmp;
	t_cmd_info	*tmp2;
	int			i;

	i = 0;
	head = *list;
	if (!*list)
		return ;
	while (head[i])
	{
		tmp2 = head[i];
		while (head[i])
		{
			tmp = head[i]->next;
			head[i]->type = 0;
			free(head[i]->data);
			free(head[i]);
			head[i] = tmp;
		}
		head[i] = tmp2;
		i++;
	}
	free(head);
}
