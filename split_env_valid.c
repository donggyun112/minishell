/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_env_valid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:34:48 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 21:42:04 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	divid_env_valid(char *str, t_cmd_info **node, t_cmd_info **head)
{
	char		**tmp;
	int			i;
	int			count;
	t_cmd_info	*node1;

	tmp = ft_split(str, ' ');
	node1 = *node;
	count = 0;
	if (tmp != NULL && tmp[1] != NULL)
	{
		i = 0;
		while (tmp[i])
			i++;
		i--;
		while (i >= 0)
		{
			push_front_t_cmd_info(node, ft_strdup(tmp[i]), count, head);
			i--;
			count++;
		}
	}
	free_split(tmp);
}

void	push_front_t_cmd_info(t_cmd_info **node, \
char *data, int num, t_cmd_info **head)
{
	t_cmd_info	*tmp;

	if (num == 0)
	{
		free((*node)->data);
		(*node)->heredoc_flag = 0;
		(*node)->type = 0;
		(*node)->data = data;
		return ;
	}
	tmp = (t_cmd_info *)malloc(sizeof(t_cmd_info));
	tmp->data = data;
	tmp->type = get_cmd_type(data);
	tmp->heredoc_flag = unexpect_token;
	tmp->next = *node;
	*node = tmp;
	*head = tmp;
}
