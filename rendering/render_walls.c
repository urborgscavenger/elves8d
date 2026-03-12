/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:18:59 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 15:43:47 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>

static void	setup_ray(t_game *game, int ray)
{
	double	camera_x;

	camera_x = 2.0 * ray / (double)NUM_RAYS - 1.0;
	game->ray.raydir_x = game->player.dir_x + game->player.plane_x * camera_x;
	game->ray.raydir_y = game->player.dir_y + game->player.plane_y * camera_x;
	game->ray.map_x = (int)game->player.x;
	game->ray.map_y = (int)game->player.y;
	game->ray.deltadist_x = fabs(1.0 / game->ray.raydir_x);
	game->ray.deltadist_y = fabs(1.0 / game->ray.raydir_y);
	if (game->ray.raydir_x < 0.0)
	{
		game->ray.step_x = -1;
		game->ray.sidedist_x = (game->player.x - game->ray.map_x)
			* game->ray.deltadist_x;
	}
	else
	{
		game->ray.step_x = 1;
		game->ray.sidedist_x = (game->ray.map_x + 1.0 - game->player.x)
			* game->ray.deltadist_x;
	}
	if (game->ray.raydir_y < 0.0)
	{
		game->ray.step_y = -1;
		game->ray.sidedist_y = (game->player.y - game->ray.map_y)
			* game->ray.deltadist_y;
	}
	else
	{
		game->ray.step_y = 1;
		game->ray.sidedist_y = (game->ray.map_y + 1.0 - game->player.y)
			* game->ray.deltadist_y;
	}
}

static void	perform_dda(t_game *game)
{
	game->ray.hit = 0;
	while (game->ray.hit == 0)
	{
		if (game->ray.sidedist_x < game->ray.sidedist_y)
		{
			game->ray.sidedist_x += game->ray.deltadist_x;
			game->ray.map_x += game->ray.step_x;
			game->ray.side = 0;
		}
		else
		{
			game->ray.sidedist_y += game->ray.deltadist_y;
			game->ray.map_y += game->ray.step_y;
			game->ray.side = 1;
		}
		if (game->ray.map_x < 0 || game->ray.map_x >= game->map.width
			|| game->ray.map_y < 0 || game->ray.map_y >= game->map.height
			|| game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
		{
			game->ray.hit = 1;
		}
	}
}

static void	draw_strip_column(t_game *game, int tex_num, double wall_x,
		int tex_y, int x_start, int x_end, int y)
{
	int	x;

	x = x_start;
	while (x < x_end)
	{
		mlx_put_pixel(game->mlx.frame.img, x, y,
			get_tex_pixel(game->tex.textures[tex_num], (int)(wall_x
					* (double)game->tex.textures[tex_num]->width), tex_y));
		x++;
	}
}

static int	compute_tex_num_and_perp(t_game *game)
{
	int	tex_num;

	if (game->ray.side == 0)
		game->ray.perpwalldist = game->ray.sidedist_x - game->ray.deltadist_x;
	else
		game->ray.perpwalldist = game->ray.sidedist_y - game->ray.deltadist_y;
	if (game->ray.side == 0)
	{
		if (game->ray.step_x > 0)
			tex_num = 2;
		else
			tex_num = 3;
	}
	else
	{
		if (game->ray.step_y > 0)
			tex_num = 1;
		else
			tex_num = 0;
	}
	return (tex_num);
}

static void	draw_strip(t_game *game, int tex_num, int ray)
{
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		y;
	int		tex_y;
	double	wall_x;
	double	step;
	double	tex_pos;

	if (game->ray.side == 0)
		wall_x = game->player.y + game->ray.perpwalldist * game->ray.raydir_y;
	else
		wall_x = game->player.x + game->ray.perpwalldist * game->ray.raydir_x;
	wall_x = wall_x - floor(wall_x);
	line_height = (int)(SCREEN_HEIGHT / game->ray.perpwalldist);
	draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_end >= SCREEN_HEIGHT)
		draw_end = SCREEN_HEIGHT - 1;
	step = (double)game->tex.textures[tex_num]->height / (double)line_height;
	tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;
	y = draw_start;
	while (y <= draw_end)
	{
		tex_y = (int)tex_pos & (game->tex.textures[tex_num]->height - 1);
		tex_pos += step;
		draw_strip_column(game, tex_num, wall_x, tex_y, ray * STRIP_WIDTH, (ray
				+ 1) * STRIP_WIDTH, y);
		y++;
	}
}

void	render_walls(t_game *game)
{
	int ray;
	int tex_num;

	ray = 0;
	while (ray < NUM_RAYS)
	{
		setup_ray(game, ray);
		perform_dda(game);
		tex_num = compute_tex_num_and_perp(game);
		draw_strip(game, tex_num, ray);
		ray++;
	}
}