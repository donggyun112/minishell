/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_push.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 21:48:13 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 20:40:28 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	push_front_t_cmd_info(t_cmd_info **node, \
char *data, int num)
{
	t_cmd_info	*tmp;
	t_cmd_info	*tmp2;
	t_cmd_info	*head;

	if (num == 0)
	{
		free((*node)->data);
		(*node)->heredoc_flag = 0;
		(*node)->type = 0;
		(*node)->data = data;
		return ;
	}
	head = (*node);
	while (num-- > 1)
		head = head->next;
	tmp2 = head->next;
	tmp = (t_cmd_info *)malloc(sizeof(t_cmd_info));
	tmp->data = data;
	tmp->type = argv;
	tmp->heredoc_flag = unexpect_token;
	tmp->next = tmp2;
	head->next = tmp;
}
