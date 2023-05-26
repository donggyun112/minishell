/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_divid_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:22:44 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 21:54:29 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quote(const char *s)
{
	int	i;
	int	_quote;
	int	__quote;

	i = 0;
	_quote = 0;
	__quote = 0;
	while (s[i])
	{
		if (s[i] == '\'')
			_quote++;
		if (s[i] == '\"')
			__quote++;
		i++;
	}
	if (_quote % 2 == 0 || __quote % 2 == 0)
		return (0);
	return (1);
}

int	is_exist__(char c, char *sep)
{
	int	i;
	int	flag;

	i = 0;
	while (sep[i])
	{
		if (c == sep[i])
			return (0);
		i++;
	}
	return (1);
}

int	is_quote(char c)
{
	if (c == '\'' || c == '\"')
		return (0);
	return (1);
}

int	quote_len(char const *s, char *c)
{
	int	i;

	i = 0;
	if (s[i] == '\'')
	{
		i++;
		while (s[i] && s[i] != '\'')
			i++;
		return (i + 1);
	}
	else
	{
		i++;
		while (s[i] && s[i] != '\"')
			i++;
		return (i + 1);
	}
}
