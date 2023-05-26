/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_func2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:11:33 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 21:40:48 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command **lst)
{
	t_command	*node;

	while ((*lst))
	{
		node = (*lst)->next;
		free_split((*lst)->cmd);
		(*lst)->infile = 0;
		(*lst)->outfile = 0;
		free(*lst);
		*lst = node;
	}
}

void	free_env(t_table *table)
{
	char	**env;
	int		i;

	env = table->envp;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	free_all(char ***tmp, t_tmp **list, t_cmd_info ***node)
{
	free_split(*tmp);
	free_list(list);
	free_node(node);
}
