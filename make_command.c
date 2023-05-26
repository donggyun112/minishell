/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:17:38 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 22:58:35 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_command(t_cmd_info *node)
{
	int	i;

	i = 0;
	while (node)
	{
		if (node->type == command || node->type == option \
		|| node->type == argv || node->type == dquote \
		|| node->type == quote)
			i++;
		node = node->next;
	}
	return (i);
}

char	**set_cmd(t_cmd_info *node)
{
	int		size;
	char	**arr;
	int		i;

	size = count_command(node);
	if (size == 0)
		return (NULL);
	else
		arr = (char **)malloc(sizeof(char *) * (size + 1));
	i = 0;
	while (i <= size)
	{
		arr[i] = NULL;
		i++;
	}
	return (arr);
}

void	make_command(t_command *cmd_list, t_cmd_info **node)
{
	int			i;
	int			count;
	t_cmd_info	*head;

	i = -1;
	while (node[++i])
	{
		head = node[i];
		cmd_list->cmd = set_cmd(node[i]);
		count = 0;
		cmd_list->num_of_cmd = i;
		while (node[i])
		{
			if (node[i]->type == command || node[i]->type == option \
			|| node[i]->type == argv || node[i]->type == dquote \
			|| node[i]->type == quote)
			{
				cmd_list->cmd[count] = ft_strdup(node[i]->data);
				count++;
			}
			node[i] = node[i]->next;
		}
		cmd_list = cmd_list->next;
		node[i] = head;
	}
}

t_command	*check_open_file(t_cmd_info **node, t_table *table)
{
	t_heredoc_fd	*h_fd;
	t_command		*cmd_list;

	h_fd = check_heredoc(node, table);
	cmd_list = check_in_out_file(node, h_fd, table);
	free_h_fd(&h_fd);
	make_command(cmd_list, node);
	return (cmd_list);
}
