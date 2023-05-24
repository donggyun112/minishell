/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:43:39 by dongkseo          #+#    #+#             */
/*   Updated: 2023/03/14 16:56:18 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *string, int c)
{
	size_t			len;
	unsigned char	cc;

	len = ft_strlen(string);
	cc = (unsigned char)c;
	while (len > 0)
	{
		if (string[len] == cc)
			return ((char *)string + len);
		len--;
	}
	if (string[len] == cc)
		return ((char *)string + len);
	return (NULL);
}
