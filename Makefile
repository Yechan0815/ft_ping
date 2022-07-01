CC			= gcc
CFLAGS		= #-Wall -Wextra -Werror

PARSER		= parser.c env.c
MAIN		= main.c

SRCS		= $(addprefix srcs/, $(MAIN)) \
			  $(addprefix srcs/parser/, $(PARSER))

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
