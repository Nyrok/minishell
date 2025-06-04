CC = cc
FLAGS = -Wall -Werror -Wextra
OUT = main.o envp_utils.o cmds/cd.o cmds/echo.o cmds/env.o cmds/export.o cmds/pwd.o cmds/unset.o cmds/check_cmds.o
CFLAGS = -Wall -Werror -Wextra -c -I .
SRC = main.c envp_utils.c cmds/cd.c cmds/echo.c cmds/env.c cmds/export.c cmds/pwd.c cmds/unset.c cmds/check_cmds.c
NAME = minishell
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(NAME): $(OUT)
	$(CC) $(OUT) -L $(LIBFT_DIR) -lft -o $(NAME) -lreadline

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
clean:
	rm -f $(OUT)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f minishell
	make fclean -C $(LIBFT_DIR)

re: fclean all
