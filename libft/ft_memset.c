/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <student.42seoul.kr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:42:24 by dongkseo          #+#    #+#             */
/*   Updated: 2023/03/14 16:53:43 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void	*ptr, int value, size_t num)
{
	size_t			i;
	unsigned char	*temp;

	temp = (unsigned char *)ptr;
	i = 0;
	while (i < num)
		temp[i++] = (unsigned char)value;
	return (ptr);
}
