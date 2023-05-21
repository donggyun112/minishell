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
# include <signal.h>
# include "libft/libft.h"
# include <stdlib.h>

typedef struct	s_table
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
	struct s_cmd_info	*next;
}	t_cmd_info;

typedef struct s_heredoc_fd
{
	int	fd;
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
	char				**cmd;
	struct s_command	*next;
}	t_command;

typedef struct s_trash
{
	t_table	*table;
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



char	**ft_split_quote(char const *s, char *c, t_table *table);
static int	check_quote(const char *s);
static int	word_count__(char const *s, char *c, t_table *table);
const char	*quote_string(const char *s, char *c, int *count);
static char	**ft_putstring__(char const *s, char *c, char **arr);
static int	string_len__(char const *s, char *c);
static int	quote_len(char const *s, char *c);
static int	is_quote(char c);
int	is_exist__(char c, char *sep);
static char	**ft_clearall(int j, char **arr);
int	string_len_(char const *s, char *c);
char	**ft_putstring_(char const *s, char *c, char **arr);
int	word_count_(char const *s, char *c);
char	**ft_split_operator(char const *s, char *c);


#define true 1
#define false 0

#endif