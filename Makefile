# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/03 15:23:16 by wilisson          #+#    #+#              #
#    Updated: 2026/03/12 18:07:16 by mbauer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -IMLX42/include
LDFLAGS = -LMLX42/build -lmlx42 -ldl -lglfw -pthread -lm

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c \
	rendering/render_walls.c \
	rendering/render_walls_setup.c \
	rendering/render_walls_draw.c \
	rendering/render_minimap.c \
	rendering/render_minimap_helpers.c \
	rendering/render_utils.c \
	rendering/movement.c \
	parsing/read_file.c \
	parsing/split_lines.c \
	parsing/parse_file.c \
	parsing/parse_lines.c \
	parsing/parse_config.c \
	parsing/parse_colors.c \
	parsing/parse_map.c \
	parsing/parse_player.c \
	parsing/validate_map.c \
	parsing/validate_connectivity.c \
	parsing/flood_fill.c \
	parsing/utils.c
OBJ = $(SRC:.c=.o)

MLX42_LIB = MLX42/build/libmlx42.a

all: $(NAME)

$(NAME): $(MLX42_LIB) $(LIBFT) $(OBJ)
	$(CC) $(OBJ) $(LIBFT) -o $(NAME) $(LDFLAGS)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

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
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean
	@if [ -d "MLX42/build" ]; then \
		rm -rf MLX42/build; \
	fi
	@if [ -d "MLX42" ]; then \
		rm -rf MLX42; \
	fi

re: fclean all

.PHONY: all clean fclean re