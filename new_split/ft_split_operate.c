/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_operate.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:41 by dongkseo          #+#    #+#             */
/*   Updated: 2023/09/09 02:47:38 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_clearall(int j, char **arr)
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

const char	*word_count_di(const char *s, char *c)
{
	char	sep;

	while (*s && is_exist__(*s, c) && !divid_cmd(s))
	{
		if (*s == '\"' || *s == '\'')
		{
			sep = *s;
			s++;
			while (*s && *s != sep)
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
