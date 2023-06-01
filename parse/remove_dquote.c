/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_dquote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:53:44 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 19:54:02 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	link_quote(t_cmd_info **node, char **tmp, int flag)
{
	int		i;
	char	*ret;
	char	*buff;

	i = 0;
	if (flag)
		return ;
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

void	*remove_if(t_cmd_info **node, t_table *table)
{
	int				i;
	t_remove_vaild	d;
	t_cmd_info		tmp2;

	d.tmp = ft_split_divid_quote((*node)->data, "");
	i = -1;
	d.flag2 = 0;
	(*node)->heredoc_flag = unexpect_token;
	while (d.tmp[++i])
	{
		d.flag = 0;
		d.tmp[i] = remove_env_dquote_2(&d.tmp[i]);
		if ((get_cmd_type(d.tmp[i]) == dquote \
		|| get_cmd_type(d.tmp[i]) == quote) \
		&& remain_single(d.tmp[i]))
			remove_quote_or_dquote(&d, node, i);
		tmp2.data = d.tmp[i];
		if (d.flag != 1 && (*node)->type != redict_in)
			re_place_get(&d, table, &tmp2, i);
		if (!d.flag)
			divid_env_valid(d.tmp[i], node, &(d.flag2));
	}
	link_quote(node, d.tmp, d.flag2);
	return (free_split(d.tmp));
}

void	remove_dquote(t_cmd_info **node, t_table *table)
{
	int			i;
	t_cmd_info	*head;

	i = 0;
	while (node[i])
	{
		head = node[i];
		while (node[i])
		{
			remove_if(&node[i], table);
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}
