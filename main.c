/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 12:00:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/11 18:22:12 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include "includes/render.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

void	free_split(char **arr)
{
	int	i = 0;
	while (arr[i]) free(arr[i++]);
	free(arr);
}

int main(int argc, char **argv)
{
	t_game game;

	if (argc != 2) {
		printf("Error\nUsage: ./cub3D map.cub\n");
		return 1;
	}

	// Initialize game structure
	game.map.grid = NULL;
	game.map.height = 0;
	game.map.width = 0;
	game.config.no_path = NULL;
	game.config.so_path = NULL;
	game.config.we_path = NULL;
	game.config.ea_path = NULL;
	game.config.floor_color = 0;
	game.config.ceiling_color = 0;

	// Parse the map file
	if (parse_file(argv[1], &game)) return 1;

	// Load textures
	char cwd[1024];
	if (!getcwd(cwd, sizeof(cwd))) {
		printf("Error\nFailed to get current directory\n");
		return 1;
	}
	char full_no[2048]; sprintf(full_no, "%s/%s", cwd, game.config.no_path);
	char full_so[2048]; sprintf(full_so, "%s/%s", cwd, game.config.so_path);
	char full_we[2048]; sprintf(full_we, "%s/%s", cwd, game.config.we_path);
	char full_ea[2048]; sprintf(full_ea, "%s/%s", cwd, game.config.ea_path);
	printf("Loading: %s\n", full_no);
	game.tex.textures[0] = mlx_load_png(full_no); // N
	game.tex.textures[1] = mlx_load_png(full_so); // S
	game.tex.textures[2] = mlx_load_png(full_we); // W
	game.tex.textures[3] = mlx_load_png(full_ea); // E
	if (!game.tex.textures[0] || !game.tex.textures[1] || !game.tex.textures[2] || !game.tex.textures[3]) {
		printf("Error\nFailed to load textures\n");
		return 1;
	}
	// Initialize MLX42
	game.mlx.mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", true);
	if (!game.mlx.mlx) {
		printf("Error\nFailed to initialize MLX42\n");
		return 1;
	}

	// Set up key hook for escape
	mlx_key_hook(game.mlx.mlx, key_hook, &game);

	// Create image for rendering
	game.mlx.frame.img = mlx_new_image(game.mlx.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!game.mlx.frame.img) {
		printf("Error\nFailed to create image\n");
		mlx_terminate(game.mlx.mlx);
		return 1;
	}

	// Draw ceiling and floor
	draw_background(&game);

	// Render walls
	render_walls(&game);

	// Render minimap
	render_minimap(&game);

	// Put image to window
	mlx_image_to_window(game.mlx.mlx, game.mlx.frame.img, 0, 0);

	// Start the game loop
	mlx_loop(game.mlx.mlx);

	// Cleanup
	for (int i = 0; i < 4; i++) {
		if (game.tex.textures[i]) {
			mlx_delete_texture(game.tex.textures[i]);
		}
	}
	free_split(game.map.grid);
	free(game.config.no_path);
	free(game.config.so_path);
	free(game.config.we_path);
	free(game.config.ea_path);

	mlx_terminate(game.mlx.mlx);
	return 0;
}