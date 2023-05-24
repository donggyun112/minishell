/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:19 by dongkseo          #+#    #+#             */
/*   Updated: 2023/03/14 16:53:22 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *source, size_t num)
{
	char		*temp;
	const char	*src;

	if (dest == source)
		return (dest);
	temp = (char *)dest;
	src = (const char *)source;
	while (num--)
		*temp++ = *src++;
	return (dest);
}
