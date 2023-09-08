/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_operate_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:30:10 by dongkseo          #+#    #+#             */
/*   Updated: 2023/09/09 02:37:09 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			s = word_count_di(s, c);
		}
		else
			s++;
	}
	return (count);
}

int	string_len_(char const *s, char *c)
{
	int	len;
	int	quote;

	len = 0;
	len = divid_cmd(s);
	quote = 0;
	if (len)
		return (len);
	while (s[len] && is_exist__(s[len], c) && !divid_cmd(&s[len]))
	{
		quote = quote_len(s, "");
		if (quote)
			len += quote;
		else
			len++;
	}
	return (len);
}
