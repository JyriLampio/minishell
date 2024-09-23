# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/16 11:05:56 by alogvine          #+#    #+#              #
#    Updated: 2024/09/23 16:04:27 by jlampio          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = main.c				\
	   builtins.c			\
	   free.c				\
	   pipesplit.c			\
	   parse_redirs.c		\
	   parse_redirs_utils.c	\
	   pipe.c				\
	   builtins_utils.c		\
	   utils.c				\
	   builtin_cd.c			\
	   pipe_utils.c			\
	   signals.c			\
	   close.c				\
	   duplicators.c		\
	   redirs.c				\
	   init.c				\
	   make_structs.c		\
	   make_structs_utils.c	\
	   check_syntax.c		\
	   expand.c				\
	   expand_utils.c		\
	   pipe_utils_2.c		\
	   pipe_utils_3.c		\
	   builtin_export.c		\
	   builtin_unset.c		\
	   builtin_env.c		\
	   builtin_pwd.c
	   
OBJ = $(SRCS:.c=.o)

LIBFT = libft/libft.a
LIBFT_DIR = libft/

CC = cc
CFLAGS = -Wall -Werror -Wextra -g #-fsanitize=memory
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

