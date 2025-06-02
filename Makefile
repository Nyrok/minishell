CX = cc
CXFLAGS = -Wall -Wextra -Werror -g3 -I ./includes

SRCS =	main.c

OBJS = $(SRCS:.c=.o)
OUTPUT = minishell

LIB_FT = libft/libft.a

all: $(OUTPUT)

$(LIB_FT):
	make -C libft

$(OUTPUT): $(OBJS) $(LIB_FT)
	$(CX) $(CXFLAGS) $(OBJS) -o $(OUTPUT) $(LIB_FT)

%.o: %.c
	$(CX) $(CXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make clean -C libft

fclean: clean
	rm -f $(OUTPUT)
	rm -f $(LIB_FT)

re: fclean all

.PHONY: all clean fclean re
