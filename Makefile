CX = cc
CXFLAGS = -Wall -Werror -Wextra -I includes -I/opt/homebrew/opt/readline/include -g3 -std=c99
LDFLAGS = -L/opt/homebrew/opt/readline/lib -lreadline
SRCS = main.c \
	envp_utils.c \
	cmds/cd.c \
	cmds/echo.c \
	cmds/env.c \
	cmds/export.c \
	cmds/pwd.c \
	cmds/unset.c \
	cmds/exit.c \
	cmds/check_cmds.c \
	executor/cmd_searcher.c \
	cmds_paths_utils.c \
	line_reader.c \
	free.c \
	history_utils.c \
	parser/tokenizer.c \
	parser/parser.c \
	parser/checker.c \
	parser/cmd_info.c \
	parser/utils.c \
	parser/redir.c

OBJDIR = objs
OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

NAME = minishell
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CX) $(OBJS) -L $(LIBFT_DIR) -lft $(LDFLAGS) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CX) $(CXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re