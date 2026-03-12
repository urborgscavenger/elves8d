/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:18:59 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 15:32:56 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>

void	render_walls(t_game *game)
{
	int		ray;
	int		tex_num;
	int		tex_width;
	int		tex_x;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		x_start;
	int		x_end;
	int		y;
	int		x;
	int		tex_y;
	double	camera_x;
	double	wall_x;
	double	step;
	double	tex_pos;
	uint32_t	color;

	ray = 0;
	while (ray < NUM_RAYS)
	{
		/* calculate ray position and direction */
		camera_x = 2.0 * ray / (double)NUM_RAYS - 1.0;
		game->ray.raydir_x = game->player.dir_x
			+ game->player.plane_x * camera_x;
		game->ray.raydir_y = game->player.dir_y
			+ game->player.plane_y * camera_x;

		game->ray.map_x = (int)game->player.x;
		game->ray.map_y = (int)game->player.y;

		/* length of ray from one x or y-side to next x or y-side */
		game->ray.deltadist_x = fabs(1.0 / game->ray.raydir_x);
		game->ray.deltadist_y = fabs(1.0 / game->ray.raydir_y);

		/* calculate step and initial sideDist */
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

		/* perform DDA */
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

			/* check bounds and hit */
			if (game->ray.map_x < 0
				|| game->ray.map_x >= game->map.width
				|| game->ray.map_y < 0
				|| game->ray.map_y >= game->map.height
				|| game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
			{
				game->ray.hit = 1;
			}
		}

		/* calculate distance projected on camera direction */
		if (game->ray.side == 0)
		{
			game->ray.perpwalldist = game->ray.sidedist_x - game->ray.deltadist_x;
		}
		else
		{
			game->ray.perpwalldist = game->ray.sidedist_y - game->ray.deltadist_y;
		}

		/* get texture index based on cardinal direction */
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

		/* calculate exactly where the wall was hit */
		if (game->ray.side == 0)
		{
			wall_x = game->player.y + game->ray.perpwalldist
				* game->ray.raydir_y;
		}
		else
		{
			wall_x = game->player.x + game->ray.perpwalldist
				* game->ray.raydir_x;
		}
		wall_x = wall_x - floor(wall_x);

		/* x coordinate on the texture */
		tex_width = game->tex.textures[tex_num]->width;
		tex_x = (int)(wall_x * (double)tex_width);

		/* fix texture flipping on specific sides */
		if (game->ray.side == 0 && game->ray.raydir_x > 0.0)
		{
			tex_x = tex_width - tex_x - 1;
		}
		if (game->ray.side == 1 && game->ray.raydir_y < 0.0)
		{
			tex_x = tex_width - tex_x - 1;
		}

		/* calculate height of line to draw on screen */
		line_height = (int)(SCREEN_HEIGHT / game->ray.perpwalldist);

		/* calculate lowest and highest pixel to fill in current stripe */
		draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0)
		{
			draw_start = 0;
		}
		draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_end >= SCREEN_HEIGHT)
		{
			draw_end = SCREEN_HEIGHT - 1;
		}

		/* calculate strip bounds (for performance/scaling) */
		x_start = ray * STRIP_WIDTH;
		x_end = (ray + 1) * STRIP_WIDTH;
		if (ray == NUM_RAYS - 1)
		{
			x_end = SCREEN_WIDTH;
		}

		/* step variable for texture mapping performance */
		step = (double)game->tex.textures[tex_num]->height / (double)line_height;
		tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

		/* draw the vertical strip */
		y = draw_start;
		while (y <= draw_end)
		{
			tex_y = (int)tex_pos & (game->tex.textures[tex_num]->height - 1);
			tex_pos += step;

			color = get_tex_pixel(game->tex.textures[tex_num], tex_x, tex_y);
			x = x_start;
			while (x < x_end)
			{
				mlx_put_pixel(game->mlx.frame.img, x, y, color);
				x++;
			}
			y++;
		}

		ray++;
	}
}