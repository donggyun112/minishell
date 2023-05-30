/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:41 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/30 23:39:11 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	string_len(char const *s, char c)
{
	int	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

char	**ft_clearall__________(int j, char **arr)
{
	while (j > 0)
		free(arr[--j]);
	free(arr);
	return (NULL);
}

char	**ft_putstring(char const *s, char c, char **arr)
{
	int	len;
	int	j;
	int	z;

	j = 0;
	while (*s)
	{
		if (*s != c)
		{
			len = string_len(s, c);
			arr[j] = (char *)malloc(len + 1);
			if (!arr[j])
				return (ft_clearall__________(j, arr));
			z = 0;
			while (len-- > 0)
				arr[j][z++] = *s++;
			arr[j][z] = '\0';
			j++;
		}
		else
			s++;
	}
	return (arr);
}

int	word_count(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		if (*s != c)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
		else
			s++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		count;

	if (!s)
		return (NULL);
	count = word_count(s, c);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr = ft_putstring(s, c, arr);
	if (!arr)
	{
		free(arr);
		return (NULL);
	}
	arr[count] = NULL;
	return (arr);
}
