CC			= gcc
CFLAGS		= #-Wall -Wextra -Werror

MAIN		= main.c
PARSER		= parser.c type.c env.c parameter.c argument.c option.c error.c free.c
ADAPTER		= convert.c time.c type.c quit.c
NETWORK		= network.c analysis.c statistics.c error.c

SRCS		= $(addprefix srcs/, $(MAIN)) \
			  $(addprefix srcs/parser/, $(PARSER)) \
			  $(addprefix srcs/adapter/, $(ADAPTER)) \
			  $(addprefix srcs/network/, $(NETWORK))

INCS		= incs/
OBJS		= $(SRCS:srcs/%.c=objs/%.o)

NAME		= ft_ping



$(OBJS)		:	objs/%.o : srcs/%.c
				$(CC) $(CFLAGS) -c $< -o $@ -I$(INCS)

all			:	$(NAME)

$(NAME)		:	$(OBJS)
				$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

clean		:
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all
