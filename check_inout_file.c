/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_inout_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:28:31 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 02:08:51 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_fd(t_command *cmd_list, int infile, int outfile)
{
	while (cmd_list->next)
		cmd_list = cmd_list->next;
	cmd_list->infile = infile;
	cmd_list->outfile = outfile;
}

void	errno_print(const char *str, t_table *table, t_fd_status *fd)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	table->exit_status = 1;
	table->fd_status = -1;
	fd->status = 0;
}

void	close_file(t_cmd_info *node, t_fd_status *fd)
{
	if (node->type == redict_in)
	{
		if (fd->in != 0)
			close(fd->in);
	}
	if (node->type == dict_in && fd->status != -1)
	{
		if (fd->in != 0)
			close(fd->in);
	}
	if (node->type == redict_out && fd->status != -1)
	{
		if (fd->out != 1)
			close(fd->out);
	}
	if (node->type == dict_out && fd->status != -1)
	{
		if (fd->out != 1)
			close(fd->out);
	}
}

void	open_in_out_file(t_cmd_info **node, t_fd_status *fd, \
	int i, t_heredoc_fd **h_fd)
{
	while (node[i])
	{
		close_file(node[i], fd);
		if (node[i]->type == redict_in)
		{
			fd->in = (*h_fd)->fd;
			*h_fd = (*h_fd)->next;
		}
		if (node[i]->type == dict_in && fd->status != -1)
		{
			fd->in = open(node[i]->data, O_RDONLY);
			if (fd->in == -1)
			{
				fd->error_file = node[i]->data;
				fd->status = -1;
			}
		}
		if (node[i]->type == redict_out && fd->status != -1)
			fd->out = open(node[i]->data, O_RDWR | O_CREAT | O_APPEND, 0644);
		if (node[i]->type == dict_out && fd->status != -1)
			fd->out = open(node[i]->data, O_RDWR | O_CREAT | O_TRUNC, 0644);
		node[i] = node[i]->next;
	}
}

t_command	\
	*check_in_out_file(t_cmd_info **node, t_heredoc_fd *h_fd, t_table *table)
{
	int				i;
	t_fd_status		fd;
	t_cmd_info		*head;
	t_command		*cmd_list;
	t_heredoc_fd	*tmp;

	i = -1;
	cmd_list = NULL;
	fd.status = 0;
	tmp = h_fd;
	while (node[++i])
	{
		head = node[i];
		fd.in = 0;
		fd.out = 1;
		push_fd(&cmd_list, 0, 1);
		open_in_out_file(node, &fd, i, &h_fd);
		if (fd.status == -1)
			errno_print(fd.error_file, table, &fd);
		replace_fd(cmd_list, fd.in, fd.out);
		node[i] = head;
	}
	h_fd = tmp;
	return (cmd_list);
}
