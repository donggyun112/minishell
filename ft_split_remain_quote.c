/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_remain_quote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:41 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 22:11:56 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

const char	*quote_string_re(const char *s)
{
	const char	*tmp;

	tmp = check_env_quote_re(s);
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



const char	*wrod_count_re(const char *s, char *c)
{
	while (*s && is_exist___re(*s, c))
	{
		if (*s == '\'')
		{
			s++;
			while (*s && *s != '\'')
				s++;
			if (*s && *s == '\'')
				s++;
		}
		else if (*s == '\"')
		{
			s++;
			while (*s && *s != '\"')
				s++;
			if (*s && *s == '\"')
				s++;
		}
		else
			s++;
	}
	return (s);
}

int	word_count___re(char const *s, char *c, t_table *table)
{
	int			count;
	const char	*his;

	count = 0;
	while (*s)
	{
		if (is_exist___re(*s, c))
		{
			count++;
			his = s;
			s = quote_string_re(s);
			if (!s)
			{
				table->syntax_error = 1;
				return (-1);
			}
			if (his != s)
				continue ;
			s = wrod_count_re(s, c);
		}
		else
			s++;
	}
	return (count);
}

int	check_quote_re(const char *s)
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

char	**ft_split_quote_re(char const *s, char *c, t_table *table)
{
	char	**arr;
	int		count;

	table->syntax_error = 0;
	count = word_count___re(s, c, table);
	if (count == -1)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr = ft_putstring___re(s, c, arr);
	if (!arr)
	{
		free(arr);
		return (NULL);
	}
	arr[count] = NULL;
	return (arr);
}
