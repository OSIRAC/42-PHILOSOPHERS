NAME        = philo
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -fsanitize=thread
SRCS        = philo.c checker.c init_sleep_print.c philo_util.c philo_util2.c
OBJS        = $(SRCS:.c=.o)
HEADER      = philo.h

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
