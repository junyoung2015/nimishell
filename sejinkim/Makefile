# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/29 11:07:07 by sejinkim          #+#    #+#              #
#    Updated: 2023/06/21 17:39:16 by sejinkim         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra
NAME = minishell
HDR_DIR = ./includes/
HDR = $(HDR_DIR)minishell.h
SRCS_MANDATORY = \
	./srcs/main.c \
	./srcs/lexer.c \
	./srcs/parser.c \
	./srcs/err.c 

OBJS_MANDATORY = $(SRCS_MANDATORY:.c=.o)
INCLUDE = -I$(HDR_DIR)

all : $(NAME)

$(NAME) : $(OBJS_MANDATORY) 
	$(CC) $(CFLAGS) $(INCLUDE) $(LIB) $^ -o $(NAME)

%.o : %.c $(HDR)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean :
	rm -f $(OBJS_MANDATORY)

fclean : 
	make clean
	rm -f $(NAME)

re :
	make clean 
	rm -f $(NAME)
	make all

.PHONY : all clean fclean re