/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 18:58:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:16:17 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include "includes/render.h"

static int	load_texture(t_game *game, int index, char *cwd)
{
	char	full_path[2048];
	char	*path;

	if (index == 0)
		path = game->config.no_path;
	else if (index == 1)
		path = game->config.so_path;
	else if (index == 2)
		path = game->config.we_path;
	else
		path = game->config.ea_path;
	sprintf(full_path, "%s/%s", cwd, path);
	if (index == 0)
		printf("Loading: %s\n", full_path);
	game->tex.textures[index] = mlx_load_png(full_path);
	return (game->tex.textures[index] == NULL);
}

void	init_game(t_game *game)
{
	int	i;

	game->map.grid = NULL;
	game->map.height = 0;
	game->map.width = 0;
	game->config.no_path = NULL;
	game->config.so_path = NULL;
	game->config.we_path = NULL;
	game->config.ea_path = NULL;
	game->config.floor_color = 0;
	game->config.ceiling_color = 0;
	game->mlx.mlx = NULL;
	game->mlx.frame.img = NULL;
	i = 0;
	while (i < 4)
	{
		game->tex.textures[i] = NULL;
		i++;
	}
}

int	load_textures(t_game *game)
{
	char	cwd[1024];
	int		index;

	if (!getcwd(cwd, sizeof(cwd)))
	{
		printf("Error\nFailed to get current directory\n");
		return (1);
	}
	index = 0;
	while (index < 4)
	{
		if (load_texture(game, index, cwd))
		{
			printf("Error\nFailed to load textures\n");
			return (1);
		}
		index++;
	}
	return (0);
}

int	init_graphics(t_game *game)
{
	game->mlx.mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", true);
	if (!game->mlx.mlx)
	{
		printf("Error\nFailed to initialize MLX42\n");
		return (1);
	}
	mlx_key_hook(game->mlx.mlx, key_hook, game);
	game->mlx.frame.img = mlx_new_image(game->mlx.mlx,
			SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!game->mlx.frame.img)
	{
		printf("Error\nFailed to create image\n");
		mlx_terminate(game->mlx.mlx);
		return (1);
	}
	draw_background(game);
	render_walls(game);
	render_minimap(game);
	mlx_image_to_window(game->mlx.mlx, game->mlx.frame.img, 0, 0);
	return (0);
}

void	cleanup_game(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->tex.textures[i])
			mlx_delete_texture(game->tex.textures[i]);
		i++;
	}
	free_tab(game->map.grid);
	free(game->config.no_path);
	free(game->config.so_path);
	free(game->config.we_path);
	free(game->config.ea_path);
	if (game->mlx.mlx)
		mlx_terminate(game->mlx.mlx);
}
