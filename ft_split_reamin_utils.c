/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_reamin_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:33:58 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 21:49:23 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_clearall_re(int j, char **arr)
{
	while (j > 0)
		free(arr[--j]);
	free(arr);
	return (NULL);
}

int	is_exist___re(char c, char *sep)
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

int	is_quote_re(char c)
{
	if (c == '\'' || c == '\"')
		return (0);
	return (1);
}

int	quote_len_re(char const *s, char *c)
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

int	env_quote_len_re(const char *s)
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