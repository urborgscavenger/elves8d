# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/03/03 15:23:16 by wilisson          #+#    #+#              #
#    Updated: 2026/03/23 22:01:16 by wilisson         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3d

CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -IMLX42/include
LDFLAGS = -LMLX42/build -lmlx42 -ldl -lglfw -pthread -lm

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = main.c \
	main_utils.c \
	rendering/render_walls.c \
	rendering/render_walls_setup.c \
	rendering/render_walls_draw.c \
	rendering/render_minimap.c \
	rendering/render_minimap_helpers.c \
	rendering/render_utils.c \
	rendering/input.c \
	rendering/movement.c \
	parsing/io/read_file.c \
	parsing/io/split_lines.c \
	parsing/io/parse_file.c \
	parsing/utils/parse_lines.c \
	parsing/config/parse_config.c \
	parsing/config/parse_colors.c \
	parsing/map/parse_map.c \
	parsing/map/parse_player.c \
	parsing/map/validate_map.c \
	parsing/map/flood_fill.c \
	parsing/utils/free_tab.c
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

%.o: %.c | $(MLX42_LIB) $(LIBFT)
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