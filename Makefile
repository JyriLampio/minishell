# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/16 11:05:56 by alogvine          #+#    #+#              #
#    Updated: 2024/09/16 11:21:51 by alogvine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c			\
	   builtins.c		\
	   free.c			\
	   pipesplit.c

OBJ = $(SRCS:.c=.o)

LIBFT = libft/libft.a
LIBFT_DIR = libft/

CC = cc
CFLAGS = -Wall -Werror -Wextra -g
LDFLAGS = -L$(LIBFT_DIR) -lft

NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS)  -o $(NAME) $(OBJ) $(LIBFT) -lreadline


clean:
	rm -rf $(OBJ)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -rf $(NAME)
	make fclean -C  $(LIBFT_DIR)

re: fclean all

.PHONY: clean re fclean all

