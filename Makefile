NAME = pipex

LIBFT = ./libft/libft.a

SRCS = src/pipex.c src/parsing.c src/utils.c \
		src/error.c src/split_pex.c 

CFLAGS = -Wall -Wextra -Werror

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS) $(LIBFT)
	@cc $(CFLAGS) $^ -o $@ $(LIBFT)
	@echo " \n$(Green) * * * * * * *  P I P E X  * * * * * * * $(Ending)\n"

$(LIBFT):
	@make -C libft/

clean:
	@rm -rf $(OBJS)
	@make clean -C libft
	@rm -rf *.dSYM
	@rm -rf .vscode
	@rm -rf .DS_Store

fclean: clean
	@rm -f $(NAME)
	@make fclean -C libft
	@echo " \n$(Yellow) * * * * * * A L L  C L E A N * * * * * * $(Ending)\n"

re: fclean all

.PHONY: all clean fclean re


Green = \033[0;32m
Yellow = \033[0;33m
Ending = \033[0m
