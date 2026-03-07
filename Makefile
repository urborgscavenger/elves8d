# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/03 15:23:16 by wilisson          #+#    #+#              #
#    Updated: 2026/03/07 11:51:21 by mbauer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -IMLX42/include
LDFLAGS = -LMLX42/build -lmlx42 -ldl -lglfw -pthread -lm

SRC = main.c
OBJ = $(SRC:.c=.o)

MLX42_LIB = MLX42/build/libmlx42.a

all: $(NAME)

$(NAME): $(MLX42_LIB) $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)

$(MLX42_LIB):
	@if [ ! -d "MLX42" ]; then \
		echo "Cloning MLX42..."; \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi
	@if [ ! -d "MLX42/build" ]; then \
		echo "Building MLX42..."; \
		cd MLX42 && cmake -B build && cmake --build build -j4; \
	fi

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)
	@if [ -d "MLX42/build" ]; then \
		rm -rf MLX42/build; \
	fi
	@if [ -d "MLX42" ]; then \
		rm -rf MLX42; \
	fi
	@if [ -d "MLX42/build" ]; then \
		rm -rf MLX42/build; \
	fi

re: fclean all

.PHONY: all clean fclean re