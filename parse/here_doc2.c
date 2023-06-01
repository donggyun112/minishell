/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 19:28:09 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/01 19:46:56 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../utils/get_next_line.h"

void	write_heredoc(char *line, t_table *table, int infile, int type)
{
	t_cmd_info	node;

	node.data = line;
	if (type != special_heredoc)
		node.data = replace_val(&node, table, 2);
	else
		node.data = NULL;
	if (!node.data)
	{
		write(infile, line, ft_strlen(line));
	}
	else
	{
		write(infile, node.data, ft_strlen(node.data));
		free(node.data);
	}	
}

char	*create_filename(void)
{
	char	*path;
	int		fileno;
	char	*join;

	fileno = 0;
	join = ft_itoa(fileno);
	path = ft_strjoin("/tmp/minishell-here-doc-", join);
	while (access(path, F_OK) == 0)
	{
		fileno++;
		free(join);
		free(path);
		join = ft_itoa(fileno);
		path = ft_strjoin("/tmp/minishell-here-doc-", join);
	}
	free(join);
	return (path);
}

void	until_here_doc_end(char *limits, t_table *table, int infile, int type)
{
	char	*line;

	while (1)
	{
		line = get_next_line(0);
		if (!line || (line[0] != '\n'
				&& ft_strncmp(line, limits, ft_strlen(line) - 1) == 0))
		{
			free(line);
			break ;
		}
		write_heredoc(line, table, infile, type);
		free(line);
	}
}

int	init_here_doc_data(char *limits, t_table *table, int type)
{
	int		fd;
	char	*file;
	int		infile;

	if (g_sig == 2)
		return (-1);
	file = create_filename();
	fd = open(file, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	infile = dup(fd);
	until_here_doc_end(limits, table, infile, type);
	close(infile);
	infile = open(file, O_RDONLY);
	close(fd);
	unlink(file);
	free(file);
	return (infile);
}
