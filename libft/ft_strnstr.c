/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:43:35 by dongkseo          #+#    #+#             */
/*   Updated: 2023/03/14 16:55:24 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *string1, const char *string2, size_t n)
{
	size_t	i;
	size_t	j;

	if (!(*string2))
		return ((char *)string1);
	j = 0;
	i = 0;
	while ((j < n) && string1[j])
	{
		if (string1[j] == *string2)
		{
			i = 0;
			while (string2[i] && string2[i] == string1[j + i])
			{
				if (i + j >= n)
					return (NULL);
				i++;
			}
			if (*(string2 + i) == '\0')
				return ((char *)(string1 + j));
		}
		j++;
	}
	return (NULL);
}
