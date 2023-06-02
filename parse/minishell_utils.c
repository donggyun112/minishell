/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:08:05 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/02 16:16:24 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_len(char *str, t_table *table)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	str = str + 1;
	if (str[i] == '?' || str[i] == '_')
		return (1);
	if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') \
	|| str[i] == '\"' || str[i] == '\'')
	{
		while (str[i] && str[i] != 32 && !(str[i] > 9 && str[i] < 13) \
		&& str[i] != '$')
		{
			if (!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] \
			<= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == '_'))
				return (i);
			i++;
		}
	}
	else if (!str[i] || str[i] == 32 || (str[i] > 9 && str[i] < 13))
		return (0);
	else
		table->syntax_error = 1;
	return (i);
}

int	check_quote_heredoc(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
			return (1);
		i++;
	}
	return (0);
}

int	check_is_quote(char *tmp)
{
	int	i;

	i = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\"' || tmp[i] == '\'')
			break ;
		i++;
	}
	if (tmp[i] && tmp[i] == '\'')
		return (1);
	return (0);
}

int	check_dq(char *tmp)
{
	int	i;

	i = 2;
	while (tmp[i] && tmp[i] != '\"' && tmp[i] != '\'')
		i++;
	if (!tmp[i])
		return (0);
	else
		return (1);
}

char	*vaild_check(int i, char **base)
{
	char	*tmp;
	char	*tmp2;

	tmp = *base;
	if (is_single(tmp))
		return (NULL);
	if ((check_d(tmp, i, '\'') || check_d(tmp, i, '\"')))
		tmp2 = ft_strdup("$");
	else
	{
		if (check_dq(&tmp[i]))
			tmp2 = ft_substr(&tmp[i + 2], 0, ft_strlen(&tmp[i + 2]) - 1);
		else
			return (NULL);
	}
	free(tmp);
	return (tmp2);
}
