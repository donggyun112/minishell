/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_divid_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:24:24 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 21:42:25 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_quote_len(const char *s)
{
	int	i;

	i = 0;
	if (s[i] == '$' && s[i + 1] == '\"')
	{
		i += 2;
		while (s[i] && s[i] != '\"')
			i++;
		return (i + 1);
	}
	else
	{
		i += 2;
		while (s[i] && s[i] != '\'')
			i++;
		return (i + 1);
	}
}

int	string_len__(char const *s, char *c)
{
	int	len;

	len = 0;
	if (s[len] == '$' && s[len + 1] && (s[len + 1] == '\"'\
	|| s[len + 1] == '\''))
		len = env_quote_len(s);
	else if (s[len] == '\'' || s[len] == '\"')
		len = quote_len(s, c);
	else
	{
		while (s[len] && is_exist__(s[len], c) && is_quote(s[len]))
		{
			if (s[len] == '$' && s[len + 1] && (s[len + 1] == '\"'\
			|| s[len + 1] == '\''))
				break ;
			len++;
		}
	}
	return (len);
}
