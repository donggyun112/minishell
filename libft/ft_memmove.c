/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:21 by dongkseo          #+#    #+#             */
/*   Updated: 2023/03/14 16:53:24 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *source, size_t num)
{
	char		*temp;
	const char	*src;

	temp = (char *)dest;
	src = (const char *)source;
	if (temp == src)
		return (dest);
	if (dest < source)
	{
		while (num--)
			*temp++ = *src++;
	}
	else
	{
		temp += num;
		src += num;
		while (num--)
			*--temp = *--src;
	}
	return (dest);
}
