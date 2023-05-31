/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_env_valid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:34:48 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 21:54:31 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
