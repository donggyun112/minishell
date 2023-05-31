/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_error_handle.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 21:01:16 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 22:01:05 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_error(char *error)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd("exit: ", 2);
	ft_putstr_fd(error, 2);
}

void	*syntax_error__(t_table *table, char ***tmp, \
	t_tmp **list, t_cmd_info ***node)
{
	free_split(*tmp);
	free_list(list);
	free_node(node);
	return (error_clear(table));
}

void	*syntax_error_split(t_table *table, char ***tmp)
{
	free_split(*tmp);
	return (error_clear(table));
}

void	*error_clear(t_table *table)
{
	table->exit_status = return_error("minishell: syntax error\n", 258);
	return (NULL);
}

int	return_error(char *msg, int error_code)
{
	ft_putstr_fd(msg, 2);
	return (error_code);
}
