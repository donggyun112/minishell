/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:31:37 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 21:50:15 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	push_heredoc_fd(t_heredoc_fd **h_fd, int fd)
{
	t_heredoc_fd	*tmp;
	t_heredoc_fd	*head;

	tmp = (t_heredoc_fd *)malloc(sizeof(t_heredoc_fd));
	tmp->fd = fd;
	tmp->next = NULL;
	if (*h_fd == NULL)
		*h_fd = tmp;
	else
	{
		head = *h_fd;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

void	push_cmd(t_cmd_info **cmd, char *data, int type)
{
	t_cmd_info	*tmp;
	t_cmd_info	*head;

	tmp = (t_cmd_info *)malloc(sizeof(t_cmd_info));
	tmp->data = data;
	tmp->type = type;
	tmp->next = NULL;
	if (*cmd == NULL)
		*cmd = tmp;
	else
	{
		head = *cmd;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

void	set_table(t_table *table)
{
	table->cmd_count = 0;
	table->fd_status = 0;
	table->syntax_error = 0;
}

void	push_fd(t_command **cmd_list, int infile, int outfile)
{
	t_command	*tmp;
	t_command	*head;

	tmp = (t_command *)malloc(sizeof(t_command));
	tmp->infile = infile;
	tmp->outfile = outfile;
	tmp->next = NULL;
	if (!*cmd_list)
		*cmd_list = tmp;
	else
	{
		head = *cmd_list;
		while (head->next)
			head = head->next;
		head->next = tmp;
	}
}

t_cmd_info	**set_cmd_list(t_table *table, t_tmp *list)
{
	int			i;
	t_cmd_info	**tmp;

	table->cmd_count = cmd_size(table, list);
	tmp = (t_cmd_info **)malloc(sizeof(t_cmd_info *) * (table->cmd_count + 1));
	tmp[table->cmd_count] = NULL;
	i = -1;
	while (++i < table->cmd_count)
		tmp[i] = NULL;	
	return (tmp);
}
