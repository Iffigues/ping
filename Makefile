# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bordenoy <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/28 19:39:36 by bordenoy          #+#    #+#              #
#    Updated: 2019/04/25 13:19:18 by bordenoy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=ft_ping
SRC=main.c help.c addr.c outil.c read.c checksum.c time.c opt.c signal.c
OBJ=$(SRC:.c=.o)
CC=gcc
INCLUDE= -I ./
all:$(NAME)
$(NAME):$(OBJ)
	$(CC) $(INCLUDE) $(OBJ) -o $(NAME)
%.o:%.c
	$(CC) $(INCLUDE) -c $< 
clean:
	rm -f *.o
fclean:clean
	rm -f $(NAME)
re:fclean all
.PHONY:all clean fclean re
