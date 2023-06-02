/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinhyeop <jinhyeop@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 15:27:41 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/06/02 15:50:22 by jinhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exit_processor_error(t_command **cmd)
{
	free_command(cmd);
	exit (1);
}

void	exit_access_error(char *file, int err_stat)
{
	char	*err_str;

	if (err_stat == PATH)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(file);
		exit(127);
	}
	else
	{
		ft_putstr_fd("minishell: ", 2);
		err_str = ft_strjoin(file, ": command not found");
		ft_putendl_fd(err_str, 2);
		exit(127);
	}
}

void	exit_no_exec_file(char *path)
{
	if (ft_strcmp(path, ".") == 0)
	{
		ft_putendl_fd("minishell: .: filename argument required", 2);
		ft_putendl_fd(".: usage: . filename [arguments]", 2);
		exit(1);
	}
	else if (ft_strcmp(path, "..") == 0)
	{
		ft_putendl_fd("minishell: ..: command not found", 2);
		exit(1);
	}
	else if (ft_strcmp(path, "/") == 0)
	{
		ft_putendl_fd("minishell: /: is a directory", 2);
		exit(1);
	}
	else
	{
		ft_putendl_fd("minishell: : command not found", 2);
		exit(1);
	}
}
