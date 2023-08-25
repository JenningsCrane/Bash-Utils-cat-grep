FLAGS = -Wall -Wextra -Werror
CC = gcc
NAME = cat

all: clean $(NAME)

$(NAME): cat.h cat.c
	$(CC) $(FLAGS) $(NAME).c -o $(NAME)

clean:
	rm -rf $(NAME)

rebuild: clean all