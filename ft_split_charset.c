/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_charset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:41 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/23 13:23:09 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_clearall(int j, char **arr)
{
	while (j > 0)
		free(arr[--j]);
	free(arr);
	return (NULL);
}

int	divid_cmd(const char *s)
{
	if (ft_strnstr(s, ">>", 2))
		return (2);
	else if (ft_strnstr(s, "<<", 2))
		return (2);
	else if (ft_strnstr(s, ">", 1))
		return (1);
	else if (ft_strnstr(s, "<", 1))
		return (1);
	else if (ft_strnstr(s, "|", 1))
		return (1);
	return (0);
}

int	string_len_(char const *s, char *c)
{
	int	len;

	len = 0;
	len = divid_cmd(s);
	if (len)
		return (len);
	while (s[len] && is_exist__(s[len], c) && !divid_cmd(&s[len]))
	{
		if (!is_quote_re(s[len]))
		{
			len++;
			while (s[len] && is_quote_re(s[len]))
				len++;
			if (!s[len])
				return (len);
		}
		len++;
	}
	return (len);
}

char	**ft_putstring_(char const *s, char *c, char **arr)
{
	int	len;
	int	j;
	int	z;

	j = 0;
	while (*s)
	{
		if (is_exist__(*s, c))
		{
			len = string_len_(s, c);
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

const char	*word_count_di(const char *s, char *c, int *count)
{
	while (*s && is_exist__(*s, c) && !divid_cmd(s))
	{
		if (!is_quote_re(*s))
		{
			s++;
			while (*s && is_quote_re(*s))
				s++;
			if (!*s)
				return (s);
			else
				s++;
		}
		else
			s++;
	}
	return (s);
}

int	word_count_(char const *s, char *c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (is_exist__(*s, c))
		{
			count++;
			if (divid_cmd(s) == 2)
			{
				s += 2;
				continue ;
			}
			else if (divid_cmd(s) == 1)
			{
				s++;
				continue ;
			}
			s = word_count_di(s, c, &count);
		}
		else
			s++;
	}
	return (count);
}

char	**ft_split_operator(char const *s, char *c)
{
	char	**arr;
	int		count;

	count = word_count_(s, c);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr = ft_putstring_(s, c, arr);
	if (!arr)
	{
		free(arr);
		return (NULL);
	}
	arr[count] = NULL;
	return (arr);
}
