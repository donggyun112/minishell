/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:56:28 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 19:30:01 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_terminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	set_origterminal(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag |= (ECHOCTL);
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
