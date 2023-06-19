CC = cc
CFLAG = -Wall -Wextra -Werror -fsanitize=address -g
NAME = libft.a
HEAD = libft.h
SRC = ft_isalpha.c ft_toupper.c ft_isdigit.c ft_tolower.c ft_isalnum.c\
ft_strchr.c ft_isascii.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c\
ft_strnstr.c ft_atoi.c ft_isprint.c ft_strlen.c ft_memset.c ft_bzero.c\
ft_memcpy.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_calloc.c ft_strdup.c\
ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c\
ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c
SRC_BO = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c
OBJ = $(SRC:.c=.o)
OBJ_BO = $(SRC_BO:.c=.o)

ifdef BONUS
	OBJ_CUR = $(OBJ_BO)
else
	OBJ_CUR = $(OBJ)
endif
#\t$(White)$< 
Black	=\033[0;30m
Red		=\033[0;31m
Green	=\033[0;32m
Yellow	=\033[0;33m
Blue	=\033[0;34m
Purple	=\033[0;35m
Cyan	=\033[0;36m
White	=\033[0;37m
Gray    = \033[0;90m
DEF_COLOR = \033[0;39m
LF = \e[1K\r
TOTAL_FILES = $(words $(SRC))
CURRENT_FILE = 1

progress_bar = printf "$(LF)$(Cyan)[$(1)/$(2)]$(DEF_COLOR) [$(Yellow)%0.1f%%$(DEF_COLOR)] $(DEF_COLOR)\b" $(shell echo "scale=1; ($(1) / $(2)) * 100" | bc); \
		printf " [%-*s%s]" 25 "$(shell printf '%0.*s' $$(($(1) * 25 / $(2))) '=========================================================================')>" ""; \
		printf "\n\033[2K$(DEF_COLOR)$(Gray)Compiling...  $< $(DEF_COLOR) \033[A\033[999C\e[?25l" \

all : $(NAME) $(HEAD)

$(NAME) : $(OBJ_CUR)
	@ar -rsc $@ $^
	@printf "\n\e[?25l";


.c.o :
	@$(CC) $(CFLAGS) -I $(HEAD) -c -o $@ $<
	@$(call progress_bar,$(CURRENT_FILE),$(TOTAL_FILES))
	@$(eval CURRENT_FILE=$(shell echo $$(($(CURRENT_FILE)+1))))

clean :
	@rm -rf *.o all bonus libft.a
	@printf "$(LF)🚧 $(Yellow)Cleaning...🚨 $(Purple)libft$(White) $(OBJ) \n$(DEF_COLOR) "

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
	make BONUS=1 $(NAME)

.PHONY : all re clean fclean bonus
