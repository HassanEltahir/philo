NAME = philo

CFLAGS = -Wall -Wextra -Werror

RM = rm -rf

SRCS = 	main.c\
		init.c\
		threads.c\
		destroy.c\
		parsing.c \
		monitor.c \

$(NAME) :
	gcc $(CFLAGS) $(SRCS) -g3 -o $(NAME) -pthread

all : $(NAME)

fclean : clean
	$(RM) $(NAME)

clean :
	$(RM) $(NAME)

re : fclean all