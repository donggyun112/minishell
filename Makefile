NAME 	= minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

# NOTE: Add files
# ================================================
SRC 								= 	minishell \
										check_inout_file  \
										check_syntax_error \
										execute \
										free_func \
										free_func2 \
										ft_exit \
										ft_split_divid_quote \
										ft_split_divid_utils \
										ft_split_divid_utils2 \
										ft_split_operate_utils \
										ft_split_operate \
										ft_split_reamin_utils \
										ft_split_remain_quote \
										ft_split_remain_utils2 \
										here_doc \
										make_cmd_list \
										make_command \
										minishell_error_handle \
										minishell_utils \
										minishell_utils2 \
										remove_dquote \
										replace_argv_to_command \
										replace_environment_variable \
										set_ \
										signal \
										syntax_interpretation
# ================================================

SRC_CUR = $(addsuffix .c, $(SRC))
LIBFT		= libft
OBJ = $(SRC_CUR:.c=.o)
HEAD 	= minishell.h

Black	=\033[0;30m
Red		=\033[0;31m
Green	=\033[0;32m
Yellow	=\033[0;33m
Blue	=\033[0;34m
Purple	=\033[0;35m
Cyan	=\033[0;36m
White	=\033[0;37m
DEF_COLOR = \033[0;39m
LF = \e[1K\r
TOTAL_FILES = $(words $(SRC_CUR))
CURRENT_FILE = 1

progress_bar = printf "$(LF)$(Cyan)[$(1)/$(2)]$(DEF_COLOR) [$(Yellow)%0.1f%%$(DEF_COLOR)] $(DEF_COLOR)\b" $(shell echo "scale=1; ($(1) / $(2)) * 100" | bc); \
		printf " [%-*s%s]" 25 "$(shell printf '%0.*s' $$(($(1) * 25 / $(2))) '=========================================================================')>" ""; \
		printf "$(DEF_COLOR) $(Green)🔰Compiling... $(NAME) $(DEF_COLOR)" \

all : $(NAME)

$(NAME) : $(OBJ) $(HEAD)
	@make -C $(LIBFT)
	@mv libft/libft.a ./
	@$(CC) $(CFLAGS) -I $(HEAD) -L../readline -lreadline -lncurses -lhistory libft.a $(OBJ) -o $(NAME)
	@printf "$(LF)"

	@echo "$(Green)==================================================$(DEF_COLOR)"
	@echo "$(Green)|        🥳  minishell compile succsess. 🥳       |$(DEF_COLOR)"
	@echo "$(Green)|-------------------------------------------------|$(DEF_COLOR)"
	@echo "$(Green)| $(DEF_COLOR)$(Red) This is our Minishell It's not bash $(DEF_COLOR)$(Green)           |$(DEF_COLOR)"
	@echo "$(Green)|                                                 |$(DEF_COLOR)"
	@echo "$(Green)===================================================$(DEF_COLOR)"

.c.o : 
	@$(CC) $(CFLAGS) -I $(HEAD) -c -o $@ $<
	@$(call progress_bar,$(CURRENT_FILE),$(TOTAL_FILES))
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))

clean :
	@rm -rf *.o all bonus libft.a
	@printf "$(LF)🚧 $(Yellow)Cleaning...🚨 $(Purple)$(NAME)$(White) $(OBJ) \n$(DEF_COLOR) "
	@make clean -C $(LIBFT)

fclean : clean
	@printf "$(LF)🚧 $(Red)Cleaning...🚨 $(White)$(NAME) \n"
	@rm -rf $(NAME)

re :
	@$(MAKE) fclean
	@sleep 0.5
	@clear
	@printf "$(Purple)>>REBUILD<<\n"
	@$(MAKE) all

bonus :
	@make $(NAME)


.PHONY : all
