NAME 	= minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror # -fsanitize=address -g3

# NOTE: Add files

# excute 폴더 안에 실행파일 나누어서 넣어주시면 되고 헤더는 ../minishell.h로 맞춰주세요
# ================================================
SRC 								= 	minishell \
										excute/execute \
										excute/ft_exit \
										new_split/ft_split_divid_quote \
										new_split/ft_split_divid_utils \
										new_split/ft_split_divid_utils2 \
										new_split/ft_split_operate_utils \
										new_split/ft_split_operate \
										new_split/ft_split_reamin_utils \
										new_split/ft_split_remain_quote \
										new_split/ft_split_remain_utils2 \
										parse/check_inout_file  \
										parse/here_doc \
										parse/here_doc2 \
										parse/check_syntax_error \
										parse/check_syntax_error2 \
										parse/make_cmd_list \
										parse/make_command \
										parse/minishell_utils \
										parse/minishell_utils2 \
										parse/minishell_utils3 \
										parse/remove_dquote \
										parse/remove \
										parse/replace_argv_to_command \
										parse/replace_environment_variable \
										parse/syntax_interpretation \
										parse/split_env_valid \
										parse/list_push \
										parse/set \
										utils/free_func \
										utils/free_func2 \
										utils/get_next_line \
										utils/get_next_line_utils \
										utils/minishell_error_handle \
										signal/signal \
# ================================================

SRC_CUR = $(addsuffix .c, $(SRC))
LIBFT		= libft
OBJ = $(SRC_CUR:.c=.o)
HEAD 				= minishell.h
HEAD_GET_NEXT_LINE 	= utils/get_next_line.h

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
	@$(CC) $(CFLAGS) -I $(HEAD) -I $(HEAD_GET_NEXT_LINE) -c -o $@ $<
	@$(call progress_bar,$(CURRENT_FILE),$(TOTAL_FILES))
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))

clean :
	@rm -rf new_split/*.o parse/*.o signal/*.o utils/*.o *.o all bonus libft.a
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
