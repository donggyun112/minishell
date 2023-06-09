/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:24:26 by dongkseo          #+#    #+#             */
/*   Updated: 2023/06/01 18:34:40 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**copy_env(char **env)
{
	char	**tmp;
	int		i;

	i = 0;
	while (env[i])
		i++;
	tmp = (char **)malloc(sizeof(char *) * (i + 1));
	if (!tmp)
		return (NULL);
	i = -1;
	while (env[++i])
		tmp[i] = ft_strdup(env[i]);
	tmp[i] = NULL;
	return (tmp);
}

int	check_whitespace(const char *command)
{
	int			i;

	i = 0;
	while (command[i])
	{
		if (command[i] != 32 && !(command[i] >= 9 && command[i] <= 13))
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 || *s2)
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (0);
}

int	cmd_size(t_tmp *list)
{
	int	count;

	count = 1;
	while (list)
	{
		if (!ft_strcmp(list->data, "|"))
			count++;
		list = list->next;
	}
	return (count);
}

char	*ft_strchr_skip_quote(const char *string, int c, int flag2)
{
	int	flag;

	flag = 0;
	while (*string != (char)c)
	{
		if (*string == '\0')
			return (0);
		if (*string == '\"')
			flag = 1;
		if (flag != 1 && *string == '\'' && flag2 != 2)
		{
			string++;
			while (*string && *string != '\'')
				string++;
			if (!*string)
				return (NULL);
		}
		string++;
	}
	return ((char *)string);
}
