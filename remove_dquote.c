/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_dquote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:53:44 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 13:54:12 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	link_quote(t_cmd_info **node, char **tmp)
{
	int		i;
	char	*ret;
	char	*buff;

	i = 0;
	free((*node)->data);
	ret = ft_strdup("");
	while (tmp[i])
	{
		buff = ft_strjoin(ret, tmp[i]);
		free(ret);
		ret = buff;
		i++;
	}
	(*node)->data = ret;
}

char	*remove_env_dquote_2(char **base)
{
	int		i;
	char	*tmp;
	char	*tmp2;

	i = 0;
	tmp = *base;
	if (!tmp[i])
		return (tmp);
	if (get_cmd_type(tmp) == quote)
		return (tmp);
	tmp2 = remove_env_dquote(tmp);
	if (tmp2)
		return (tmp2);
	while (tmp[i + 1])
	{
		if (tmp[i] == '$' && (tmp[i + 1] == '\"'))
		{
			tmp2 = vaild_check(i, &tmp);
			if (!tmp2)
				break ;
			return (tmp2);
		}
		i++;
	}
	return (tmp);
}

int	remain_single(char *str)
{
	int		i;
	char	tar;

	i = 1;
	tar = str[0];
	while (str[i])
	{
		if (str[i] == tar)
			return (1);
		i++;
	}
	return (0);
}

void	push_front_t_cmd_info(t_cmd_info **node, char *data, int num)
{
	t_cmd_info *tmp;

	tmp = (t_cmd_info *)malloc(sizeof(t_cmd_info));
	tmp->data = data;
	tmp->type = get_cmd_type(data);
	tmp->heredoc_flag = unexpect_token;
	if (num == 0)
	{
		free((*node)->data);
		(*node)->heredoc_flag = 0;
		(*node)->type = 0;
		*node = tmp;
	}
	else
	{
		tmp->next = *node;
		*node = tmp;
	}
}

void	divid_env_valid(char *str, t_cmd_info **node)
{
	char	**tmp;
	int		i;
	int		count;

	if (get_cmd_type(str) == dquote || get_cmd_type(str) == quote)
		return ;
	tmp = ft_split(str, 32);
	count = 0;
	if (tmp != NULL && tmp[1] != NULL)
	{
		i = 0;
		while (tmp[i])
			i++;
		i--;
		while (i > 0)
		{
			push_front_t_cmd_info(node, tmp[i], count);
			i--;
			count++;
		}
	}
	free_split(tmp);
}

void	*remove_if(t_cmd_info *node)
{
	char	*base;
	char	**tmp;
	char	*ret;
	int		i;

	base = node->data;
	tmp = ft_split_divid_quote(base, "");
	i = -1;
	node->heredoc_flag = unexpect_token;
	while (tmp[++i])
	{
		tmp[i] = remove_env_dquote_2(&tmp[i]);
		if ((get_cmd_type(tmp[i]) == dquote || get_cmd_type(tmp[i]) == quote) \
		&& remain_single(tmp[i]))
		{
			if (node->type == redict_in)
				node->heredoc_flag = special_heredoc;
			ret = ft_substr(tmp[i], 1, ft_strlen(tmp[i]) - 2);
			free(tmp[i]);
			tmp[i] = ret;
		}
	//	divid_env_valid(tmp[i], &node);
	}
	link_quote(&node, tmp);
	return (free_split(tmp));
}

void	remove_dquote(t_cmd_info **node)
{
	int			i;
	t_cmd_info	*head;

	i = 0;
	while (node[i])
	{
		head = node[i];
		while (node[i])
		{
			remove_if(node[i]);
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}
