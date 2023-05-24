/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_remain_quote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:41 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/25 02:51:24 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_clearall_re(int j, char **arr)
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

static int	quote_len_re(char const *s, char *c)
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

static int	string_len___re(char const *s, char *c)
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
	//}
	return (len);
}

static char	**ft_putstring___re(char const *s, char *c, char **arr)
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

const char	*quote_string_re(const char *s, char *c, int *count)
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
		if (!is_quote_re(*s))
		{
			s++;
			while (*s && is_quote_re(*s))
				s++;
			if (*s && !is_quote_re(*s))
				s++;
		}
		else
			s++;
	}
	return (s);
}

static int	word_count___re(char const *s, char *c, t_table *table)
{
	int		count;
	const char	*his;

	count = 0;
	while (*s)
	{
		if (is_exist___re(*s, c))
		{
			count++;
			his = s;
			s = quote_string_re(s, c, &count);
			if (!s)
			{
				table->syntax_error = 1;
				return (-1);
			}
			if (his != s)
				continue;
			s = wrod_count_re(s, c);
		}
		else
			s++;
	}
	return (count);
}

static int	check_quote_re(const char *s)
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

	if (check_quote(s))
	{
		table->syntax_error = 1;
		return (NULL);
	}
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
