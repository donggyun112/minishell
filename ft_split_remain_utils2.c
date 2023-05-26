/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_remain_utils2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:36:12 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 22:58:22 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	string_len___re(char const *s, char *c)
{
	int	len;

	len = 0;
	while (s[len] && is_exist___re(s[len], c))
	{
		if ((s[len]) == '\'')
		{
			len++;
			while (s[len] && s[len] != '\'')
				len++;
			if (!s[len])
				return (len);
		}
		else if (s[len] == '\"')
		{
			len++;
			while (s[len] && s[len] != '\"')
				len++;
			if (!s[len])
				return (len);
		}
		len++;
	}
	return (len);
}

char	**ft_putstring___re(char const *s, char *c, char **arr)
{
	int	len;
	int	j;
	int	z;

	j = 0;
	while (*s)
	{
		if (is_exist___re(*s, c))
		{
			len = string_len___re(s, c);
			arr[j] = (char *)malloc(len + 1);
			if (!arr[j])
				return (ft_clearall_re(j, arr));
			z = 0;
			while (len-- > 0)
				arr[j][z++] = *s++;
			arr[j][z] = '\0';
			j++;
		}
		else
			s++;
	}
	arr[j] = NULL;
	return (arr);
}

const char	*check_env_quote_re(const char *s)
{
	if (*(s + 1))
	{
		if (*s == '$' && *(s + 1) == '\"')
		{
			s += 2;
			while (*s && *s != '\"')
				s++;
			if (*s == '\0')
				return (NULL);
			return (s + 1);
		}
		if (*s == '$' && *(s + 1) == '\'')
		{
			s += 2;
			while (*s && *s != '\'')
				s++;
			if (*s == '\0')
				return (NULL);
			return (s + 1);
		}
	}
	return (NULL);
}
