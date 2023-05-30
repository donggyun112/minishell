/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:56:28 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 04:36:56 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"

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

	if (g_sig == 2)
		return (-1);
	fd = open("/tmp/sh-thd-1641928925", \
	O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	infile = dup(fd);
	while (1)
	{
		line = get_next_line(0);
		if (!line || ft_strncmp(line, limits, ft_strlen(line) - 1) == 0)
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

void	set_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

t_heredoc_fd	*check_heredoc(t_cmd_info **node, t_table *table)
{
	int				i;
	int				fd;
	t_cmd_info		*head;
	t_heredoc_fd	*h_fd;

	set_terminal();
	i = -1;
	h_fd = NULL;
	while (node[++i])
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
	}
	return (h_fd);
}
