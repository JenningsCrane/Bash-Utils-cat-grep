FLAGS = -Wall -Wextra -Werror
CC = gcc
NAME = grep

all: clean $(NAME)

$(NAME): grep.h grep.c
	$(CC) $(FLAGS) $(NAME).c -o $(NAME)

clean:
	rm -rf $(NAME)

rebuild: clean all