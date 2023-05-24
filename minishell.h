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

typedef struct s_table
{
	char				**envp;
	int					exit_status;
	int					syntax_error;
	int					cmd_count;
	int					fd_status;
}	t_table;

typedef struct s_cmd_info
{
	char				*data;
	int					type;
	int					flag;
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
}	t_type;

typedef struct s_fd
{
	int	fd[2];
	int	std_fd[2];
}	t_fd;



int	check_quote(const char *s);
static int	word_count__(char const *s, char *c, t_table *table);
const char	*quote_string(const char *s, char *c, int *count);
static char	**ft_putstring__(char const *s, char *c, char **arr);
static int	string_len__(char const *s, char *c);
static int	quote_len(char const *s, char *c);
static int	is_quote(char c);
int			is_exist__(char c, char *sep);
static char	**ft_clearall(int j, char **arr);
int			string_len_(char const *s, char *c);
char		**ft_putstring_(char const *s, char *c, char **arr);
int			word_count_(char const *s, char *c);
char		**ft_split_operator(char const *s, char *c);
void		execute(t_command **cmd, char **envp);
void		free_command(t_command **lst);
int			ft_strcmp(char *s1, char *s2);

static char	**ft_clearall_re(int j, char **arr);
int	is_exist___re(char c, char *sep);
int	is_quote_re(char c);
static int	quote_len_re(char const *s, char *c);
int	env_quote_len_re(const char *s);
static int	string_len___re(char const *s, char *c);
static char	**ft_putstring___re(char const *s, char *c, char **arr);
const char	*check_env_quote_re(const char *s);
const char	*quote_string_re(const char *s, char *c, int *count);
const char	*wrod_count_re(const char *s, char *c);
static int	word_count___re(char const *s, char *c, t_table *table);
static int	check_quote_re(const char *s);
char	**ft_split_divid_quote(char const *s, char *c, t_table *table);
char	**ft_split_quote_re(char const *s, char *c, t_table *table);
# define true 1
# define false 0

// # define ECHO 0
// # define CD 1
// # define PWD 2
// # define EXPORT 3
// # define UNSET 4
// # define ENV 5
// # define EXIT 6

#endif
