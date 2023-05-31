/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongkseo <dongkseo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 20:32:24 by dongkseo          #+#    #+#             */
/*   Updated: 2023/05/31 12:47:51 by dongkseo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include "libft/libft.h"
# include <stdlib.h>
# include <termios.h>

# define PATH 0
# define CMD 1

int	g_sig;

typedef struct s_table
{
	char		**envp;
	int			exit_status;
	int			syntax_error;
	int			cmd_count;
	int			fd_status;
	int			save_fd;
	int			builtin_exit;
}	t_table;

typedef struct s_cmd_info
{
	char				*data;
	int					type;
	int					heredoc_flag;
	struct s_cmd_info	*next;
}	t_cmd_info;

typedef struct s_heredoc_fd
{
	int					fd;
	struct s_heredoc_fd	*next;
}	t_heredoc_fd;

typedef struct s_tmp
{
	char			*data;
	struct s_tmp	*next;
}	t_tmp;

typedef struct s_command
{
	int					infile;
	int					outfile;
	int					num_of_cmd;
	char				**cmd;
	struct s_command	*next;
}	t_command;

typedef struct s_trash
{
	t_table			*table;
	struct s_trash	*next;
}	t_trash;

typedef struct s_fd_status
{
	int		status;
	int		in;
	int		out;
	char	*error_file;
}	t_fd_status;

typedef struct s_replace
{
	char	*base;
	char	*tmp;
	char	*tmp2;
	char	*tmp3;
	char	*head;
	char	*tail;
	char	*tar;
	char	*ret;
}	t_replace;

typedef enum s_type
{
	command = 0,
	option,
	argv,
	string,
	redict_out,
	redict_in,
	dict_out,
	dict_in,
	dquote,
	quote,
	pipe_,
	unexpect_token,
	special_heredoc,
}	t_type;

typedef struct s_fd
{
	int	fd[2];
	int	std_fd[2];
}	t_fd;

// split_divid

int				check_quote(const char *s);
char			**ft_clearall(int j, char **arr);
int				is_exist__(char c, char *sep);
int				is_quote(char c);
int				quote_len(char const *s, char *c);
int				string_len__(char const *s, char *c);
int				env_quote_len(const char *s);
char			**ft_putstring__(char const *s, char *c, char **arr);
const char		*check_env_quote(const char *s);
const char		*quote_string(const char *s);
int				word_count__(char const *s, char *c);
char			**ft_split_divid_quote(char const *s, char *c);

// split_operator

const char		*word_count_di(const char *s, char *c);
int				string_len_(char const *s, char *c);
int				word_count_(char const *s, char *c);
int				divid_cmd(const char *s);
char			**ft_putstring_(char const *s, char *c, char **arr);
char			**ft_split_operator(char const *s, char *c);

// split_remain

char			**ft_clearall_re(int j, char **arr);
int				is_exist___re(char c, char *sep);
int				is_quote_re(char c);
int				quote_len_re(char const *s);
int				env_quote_len_re(const char *s);
int				string_len___re(char const *s, char *c);
char			**ft_putstring___re(char const *s, char *c, char **arr);
const char		*check_env_quote_re(const char *s);
const char		*quote_string_re(const char *s);
const char		*wrod_count_re(const char *s, char *c);
int				word_count___re(char const *s, char *c, t_table *table);
int				check_quote_re(const char *s);
char			**ft_split_quote_re(char const *s, char *c, t_table *table);

// make cmd list

t_tmp			*split_divid_operator(char **tmp1, t_table *table);
void			push_tmp(t_tmp **list, char *data);

// syntax_interpretation

int				get_cmd_type(char *cmd);
char			*remove_env_dquote(char *tmp);
t_cmd_info		**syntax_interpretation(t_tmp *list, t_table *table);
int				check_operator(int type);
int				check_unexpect_operator(t_tmp **list, t_table *table);

// check_syntax_error

int				check_operator_(t_cmd_info *node, t_table *table);
int				check_quote_count2(char *base, int *i, char tar);
int				check_quote_count(char *base);
void			check_syntax_error(t_cmd_info **node, t_table *table);
int				skip_quote(t_cmd_info *node, int *j);

// replace_argv_to_command

void			replace_argv_to_command(t_cmd_info **cmd);

// replace_environment_variable

void			replace_val4(t_replace *d, int len);
void			replace_val3(t_replace *d, t_table *table);
char			*replace_val2(t_replace *d, int len);
char			*replace_val(t_cmd_info	*node, t_table *table);
void			replace_environment_variable(t_cmd_info	**node, t_table *table);

// remove_quote

char			*remove_env_dquote_2(char **base);
void			*remove_if(t_cmd_info *node);
void			remove_dquote(t_cmd_info **node);
void			link_quote(t_cmd_info **node, char **tmp);

// check_open_file

void			write_heredoc(char *line, t_table *table, int infile, int type);
int				init_here_doc_data(char *limits, t_table *table, int type);
t_heredoc_fd	*check_heredoc(t_cmd_info **node, t_table *table);
t_command		*check_open_file(t_cmd_info **node, t_table *table);
void			push_heredoc_fd(t_heredoc_fd **h_fd, int fd);

// minishell_error_handle

void			exit_error(char *error);
void			*syntax_error__(t_table *table, char ***tmp, \
	t_tmp **list, t_cmd_info ***node);
void			*syntax_error_split(t_table *table, char ***tmp);
void			*error_clear(t_table *table);
int				return_error(char *msg, int error_code);

// free_func

void			*free_split(char **base);
void			*free_return_null(t_replace *d);
void			free_h_fd(t_heredoc_fd **h_fd);
void			free_list(t_tmp	**list);
void			free_node(t_cmd_info ***list);

// free_func2
void			free_env(t_table *table);
void			free_command(t_command **lst);
void			free_all(char ***tmp, t_tmp **list, t_cmd_info ***node);

// ft_exit

void			ft_exit(t_command *command, t_table *table);
void			exit_status(t_table *table, int j, t_command *command);
int				command_size(t_command *command);

// signal
void			set_signal(void);
void			handler_quit(int signal);
void			handler_int(int signal);
void			get_original_signal(void);

// make_command

int				count_command(t_cmd_info *node);
char			**set_cmd(t_cmd_info *node);
void			make_command(t_command *cmd_list, t_cmd_info **node);

// minishell_utils

int				env_len(char *str, t_table *table);
int				check_is_quote(char *tmp);
int				check_dq(char *tmp);
char			*vaild_check(int i, char **base);
int				check_quote_heredoc(char *line);

// minishell_utils2
int				ft_strcmp(char *s1, char *s2);
int				check_whitespace(const char *command);
char			**copy_env(char **env);

// check_inout_file

t_command	\
	*check_in_out_file(t_cmd_info **node, t_heredoc_fd *h_fd, t_table *table);
void			open_in_out_file(t_cmd_info **node, t_fd_status *fd, \
	int i, t_heredoc_fd **h_fd);
void			replace_fd(t_command *cmd_list, int infile, int outfile);
void			errno_print(const char *str, t_table *table, t_fd_status *fd);
void			close_file(t_cmd_info *node, t_fd_status *fd);

// set_
void			push_heredoc_fd(t_heredoc_fd **h_fd, int fd);
void			push_cmd(t_cmd_info **cmd, char *data, int type);
void			set_table(t_table *table);
void			push_fd(t_command **cmd_list, int infile, int outfile);
int				cmd_size(t_tmp *list);
t_cmd_info		**set_cmd_list(t_table *table, t_tmp *list);

// readline

void			rl_replace_line(const char *text, int clear_undo);

// set2
void			check_sig(int fd, t_table *table);
void			init_env_and_exit_status(t_table *table, char **env);

//execute
char			*ft_getenv(char *name, char **envp);
void			execute(t_command **cmd, t_table *table);

#endif
