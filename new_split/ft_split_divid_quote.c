/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_divid_quote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 03:02:40 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 21:52:10 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_putstring__(char const *s, char *c, char **arr)
{
	int	len;
	int	j;
	int	z;

	j = 0;
	while (*s)
	{
		if (is_exist__(*s, c))
		{
			len = string_len__(s, c);
			arr[j] = (char *)malloc(len + 1);
			if (!arr[j])
				return (ft_clearall(j, arr));
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

const char	*check_env_quote(const char *s)
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

const char	*quote_string(const char *s)
{
	const char	*tmp;

	tmp = check_env_quote(s);
	if (tmp)
		return (tmp);
	if (*s == '\'')
	{
		s++;
		while (*s && *s != '\'')
			s++;
		if (*s == '\0')
			return (NULL);
		return (s + 1);
	}
	else if (*s == '\"')
	{
		s++;
		while (*s && *s != '\"')
			s++;
		if (*s == '\0')
			return (NULL);
		return (s + 1);
	}
	else
		return (s);
}

int	word_count__(char const *s, char *c)
{
	int			count;
	const char	*his;

	count = 0;
	while (*s)
	{
		if (is_exist__(*s, c))
		{
			count++;
			his = s;
			s = quote_string(s);
			if (!s)
				return (count);
			if (his != s)
				continue ;
			while (*s && is_exist__(*s, c) && is_quote(*s))
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_split_divid_quote(char const *s, char *c)
{
	char	**arr;
	int		count;

	count = word_count__(s, c);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr = ft_putstring__(s, c, arr);
	if (!arr)
	{
		free(arr);
		return (NULL);
	}
	arr[count] = NULL;
	return (arr);
}
