/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_dquote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:53:44 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 16:22:43 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	*remove_if(t_cmd_info **node, t_cmd_info **head)
{
	char	**tmp;
	char	*ret;
	int		i;

	tmp = ft_split_divid_quote((*node)->data, "");
	i = -1;
	(*node)->heredoc_flag = unexpect_token;
	while (tmp[++i])
	{
		tmp[i] = remove_env_dquote_2(&tmp[i]);
		if ((get_cmd_type(tmp[i]) == dquote || get_cmd_type(tmp[i]) == quote) \
		&& remain_single(tmp[i]))
		{
			if ((*node)->type == redict_in)
				(*node)->heredoc_flag = special_heredoc;
			ret = ft_substr(tmp[i], 1, ft_strlen(tmp[i]) - 2);
			free(tmp[i]);
			tmp[i] = ret;
		}
		else
			divid_env_valid(tmp[i], node, head);
	}
	link_quote(node, tmp);
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
			remove_if(&node[i], &head);
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}
