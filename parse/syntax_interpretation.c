/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_interpretation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:41:10 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 02:32:43 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_unexpect_operator(t_tmp **list, t_table *table)
{
	*list = (*list)->next;
	if (!*list)
	{
		table->syntax_error = 2;
		return (1);
	}
	if (check_operator(get_cmd_type((*list)->data)))
		table->syntax_error = 1;
	return (0);
}

int	check_operator(int type)
{
	if ((type == redict_out || type == redict_in) || \
		(type == dict_in || type == dict_out))
		return (1);
	else if (type == pipe_)
		return (2);
	return (0);
}

int	get_cmd_type(char *cmd)
{
	if (!ft_strcmp(">>", cmd))
		return (redict_out);
	if (!ft_strcmp("<<", cmd))
		return (redict_in);
	if (!ft_strcmp(">", cmd))
		return (dict_out);
	if (!ft_strcmp("<", cmd))
		return (dict_in);
	if (!ft_strncmp("\"", cmd, 1))
		return (dquote);
	if (!ft_strncmp("\'", cmd, 1))
		return (quote);
	if (!ft_strcmp("|", cmd))
		return (pipe_);
	if (!ft_strncmp("-", cmd, 1))
		return (option);
	if (!ft_strcmp("(", cmd))
		return (unexpect_token);
	if (!ft_strcmp(";", cmd))
		return (unexpect_token);
	if (!ft_strcmp(")", cmd))
		return (unexpect_token);
	return (argv);
}

char	*remove_env_dquote(char *tmp)
{
	int		i;
	char	*ret;

	i = 0;
	if (!tmp[i])
		return (NULL);
	if (get_cmd_type(tmp) == quote)
		return (NULL);
	if (tmp[i] == '$' && (tmp[i + 1] == '\"' || tmp[i + 1] == '\''))
	{
		if (tmp[ft_strlen(tmp) - 2] != '\"' && tmp[ft_strlen(tmp) - 2] != '\'')
			return (NULL);
		ret = ft_substr(&tmp[i + 2], 0, ft_strlen(&tmp[i + 2]) - 1);
		free(tmp);
		return (ret);
	}
	return (NULL);
}

t_cmd_info	**syntax_interpretation(t_tmp *list, t_table *table)
{
	t_cmd_info	**tmp;
	int			type;
	int			i;

	i = 0;
	if (!list || get_cmd_type(list->data) == pipe_)
		return (NULL);
	tmp = set_cmd_list(table, list);
	while (list)
	{
		type = get_cmd_type(list->data);
		if (check_operator(type) == 1)
			if (check_unexpect_operator(&list, table))
				break ;
		if (type != pipe_)
			push_cmd(&tmp[i], ft_strdup(list->data), type);
		else
		{
			if (find_pipe(table, list))
				return (NULL);
			i++;
		}
		list = list->next;
	}
	return (tmp);
}
