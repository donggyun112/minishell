/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_operate_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:30:10 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/26 20:30:43 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			else
				len++;
		}
		else
			len++;
	}
	return (len);
}
