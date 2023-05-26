/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:56:28 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 22:56:35 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc(char *line, t_table *table, int infile, int type)
{
	t_cmd_info	node;
	char		*tmp;

	node.data = line;
	if (type != special_heredoc)
		node.data = replace_val(&node, table);
	else
		node.data = NULL;
	if (!node.data)
	{
		tmp = ft_strjoin(line, "\n");
		write(infile, tmp, ft_strlen(tmp));
		free(tmp);
	}
	else
	{
		tmp = ft_strjoin(node.data, "\n");
		write(infile, tmp, ft_strlen(tmp));
		free(node.data);
		free(tmp);
	}	
}

int	init_here_doc_data(char *limits, t_table *table, int type)
{
	int		fd;
	char	*line;
	int		infile;

	fd = open("/tmp/sh-thd-1641928925", \
	O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	infile = dup(fd);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limits) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc(line, table, infile, type);
		free(line);
	}
	close(infile);
	infile = open("/tmp/sh-thd-1641928925", O_RDONLY);
	close(fd);
	unlink("/tmp/sh-thd-1641928925");
	return (infile);
}

t_heredoc_fd	*check_heredoc(t_cmd_info **node, t_table *table)
{
	int				i;
	int				fd;
	t_cmd_info		*head;
	t_heredoc_fd	*h_fd;

	i = 0;
	h_fd = NULL;
	while (node[i])
	{
		head = node[i];
		fd = 0;
		while (node[i])
		{
			if (node[i]->type == redict_in)
			{
				fd = init_here_doc_data(node[i]->data, \
				table, node[i]->heredoc_flag);
				push_heredoc_fd(&h_fd, fd);
			}
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
	return (h_fd);
}
