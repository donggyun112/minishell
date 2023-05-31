/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:45:52 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 23:13:41 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	skip_quote(t_cmd_info *node, int *j)
{
	if (node->data[*j] == '\'')
	{
		*j += 1;
		while (node->data[*j] && node->data[*j] != '\'')
			*j += 1;
		if (!node->data[*j])
			return (1);
	}
	else if (node->data[*j] == '\"')
	{
		*j += 1;
		while (node->data[*j] && node->data[*j] != '\"')
			*j += 1;
		if (!node->data[*j])
			return (1);
	}
	return (0);
}

int	check_operator_(t_cmd_info *node, t_table *table)
{
	int			j;
	t_cmd_info	*head;

	j = 0;
	head = node;
	while (node->data[j])
	{
		if (node->data[j] == '\'' || node->data[j] == '\"')
			if (skip_quote(node, &j))
				return (0);
		if (node->data[j] == ')' || node->data[j] == '(' \
		|| node->data[j] == ';' || node->data[j] == '\\' \
		|| node->data[j] == '&')
		{
			table->syntax_error = 1;
			node = head;
			return (1);
		}
		j++;
	}
	return (0);
}

int	check_quote_count2(char *base, int *i, char tar)
{
	while (base[*i] && base[*i] != tar)
		*i += 1;
	if (!base[*i])
		return (1);
	else
		return (0);
}

int	check_quote_count(char *base)
{
	int		i;

	i = 0;
	while (base[i])
	{
		if (base[i] == '\"')
		{
			i++;
			if (check_quote_count2(base, &i, '\"'))
				return (1);
			i++;
		}
		else if (base[i] == '\'')
		{
			i++;
			if (check_quote_count2(base, &i, '\''))
				return (1);
			i++;
		}
		else
			i++;
	}
	return (0);
}

void	check_syntax_error(t_cmd_info **node, t_table *table)
{
	int			i;
	t_cmd_info	*head;

	i = 0;
	if (!node)
		return ;
	while (node[i])
	{
		head = node[i];
		while (node[i])
		{
			if (check_quote_count(node[i]->data))
			{
				node[i] = head;
				table->syntax_error = 1;
				return ;
			}
			if (node[i]->type != dquote && node[i]->type != quote)
				if (check_operator_(node[i], table))
					return ;
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}
