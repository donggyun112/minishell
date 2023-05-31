/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_environment_variable.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:49:14 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 21:41:14 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_val4(t_replace *d, int len)
{
	d->tmp2 = ft_strjoin(d->head, d->tar);
	free(d->head);
	free(d->tar);
	d->tmp3 = d->ret;
	d->ret = ft_strjoin(d->tmp3, d->tmp2);
	free(d->tmp2);
	free(d->tmp3);
	if (len == 0)
	{
		d->tmp2 = ft_strjoin(d->ret, "$");
		free(d->ret);
		d->ret = d->tmp2;
	}
	if (ft_strlen(d->tmp) >= (size_t)(len + 1))
		d->base = d->tmp + len + 1;
	else
		d->base = NULL;
}

void	replace_val3(t_replace *d, t_table *table)
{
	if (!ft_strcmp(d->tar, "?"))
	{
		free(d->tar);
		d->tar = ft_itoa(table->exit_status);
		return ;
	}
	d->tmp2 = d->tar;
	d->tar = ft_getenv(d->tar, table->envp);
	free(d->tmp2);
	d->tmp2 = NULL;
	if (!d->tar)
		d->tar = ft_strdup("");
	else
		d->tar = ft_strdup(d->tar);
}

char	*replace_val2(t_replace *d, int len)
{
	if (len == -1 && (!d->base || !*(d)->base))
		return (d->ret);
	d->tail = ft_strdup(d->base);
	d->tmp = ft_strjoin(d->ret, d->tail);
	free(d->tail);
	free(d->ret);
	return (d->tmp);
}

char	*replace_val(t_cmd_info	*node, t_table *table)
{
	t_replace	d;
	int			len;
	int			i;

	d.base = node->data;
	i = 0;
	if (node->type == redict_in)
		return (NULL);
	d.ret = ft_strdup("");
	while (1)
	{
		d.tmp = ft_strchr_skip_quote(d.base, '$');
		if (i++ == 0 && (!(d.tmp) || !*(d.tmp + 1)))
			return (free_return_null(&d));
		len = env_len(d.tmp, table);
		if (table->syntax_error)
			return (NULL);
		if (!d.tmp)
			return (replace_val2(&d, len));
		d.head = ft_substr(d.base, 0, d.tmp - d.base);
		d.tar = ft_substr(d.tmp + 1, 0, len);
		if (len != 0)
			replace_val3(&d, table);
		replace_val4(&d, len);
	}
}

void	replace_environment_variable(t_cmd_info	**node, t_table *table)
{
	int			i;
	char		*tmp;
	t_cmd_info	*head;

	i = 0;
	while (node[i])
	{
		head = node[i];
		tmp = NULL;
		while (node[i])
		{
			tmp = replace_val(node[i], table);
			if (tmp && node[i]->type != quote)
			{
				free(node[i]->data);
				node[i]->data = tmp;
			}
			node[i] = node[i]->next;
		}
		node[i] = head;
		i++;
	}
}
