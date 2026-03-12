/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 18:22:17 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:22:23 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>
#include <stdlib.h>

typedef struct s_raydraw
{
	double	cam_x;
	double	ray_x;
	double	ray_y;
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		end_x;
	int		end_y;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	int		x;
	int		y;
	int		move_x;
	int		hit;
}	t_raydraw;

static void	init_ray(t_game *game, t_raydraw *ray_data, int ray)
{
	ray_data->cam_x = 2 * (double)ray / (NUM_RAYS - 1) - 1;
	ray_data->ray_x = game->player.dir_x
		+ game->player.plane_x * ray_data->cam_x;
	ray_data->ray_y = game->player.dir_y
		+ game->player.plane_y * ray_data->cam_x;
	ray_data->map_x = (int)game->player.x;
	ray_data->map_y = (int)game->player.y;
	ray_data->delta_x = fabs(1 / ray_data->ray_x);
	ray_data->delta_y = fabs(1 / ray_data->ray_y);
	ray_data->step_x = (ray_data->ray_x >= 0) - (ray_data->ray_x < 0);
	ray_data->step_y = (ray_data->ray_y >= 0) - (ray_data->ray_y < 0);
	ray_data->side_x = ((ray_data->ray_x < 0)
			* (game->player.x - ray_data->map_x)
			+ (ray_data->ray_x >= 0)
			* (ray_data->map_x + 1 - game->player.x))
		* ray_data->delta_x;
	ray_data->side_y = ((ray_data->ray_y < 0)
			* (game->player.y - ray_data->map_y)
			+ (ray_data->ray_y >= 0)
			* (ray_data->map_y + 1 - game->player.y))
		* ray_data->delta_y;
	ray_data->hit = 0;
}

static void	advance_ray(t_raydraw *ray_data)
{
	ray_data->e2 = 2 * ray_data->err;
	if (ray_data->e2 > -ray_data->dy)
	{
		ray_data->err -= ray_data->dy;
		ray_data->x += ray_data->sx;
	}
	if (ray_data->e2 < ray_data->dx)
	{
		ray_data->err += ray_data->dx;
		ray_data->y += ray_data->sy;
	}
}

static void	draw_ray_line(t_game *game, t_minimap *mini, t_raydraw *ray_data)
{
	ray_data->end_x = mini->offset_x + ray_data->map_x * mini->cell_size
		+ mini->cell_size / 2;
	ray_data->end_y = mini->offset_y + ray_data->map_y * mini->cell_size
		+ mini->cell_size / 2;
	ray_data->x = mini->px;
	ray_data->y = mini->py;
	ray_data->dx = abs(ray_data->end_x - ray_data->x);
	ray_data->dy = abs(ray_data->end_y - ray_data->y);
	ray_data->sx = (ray_data->x < ray_data->end_x) * 2 - 1;
	ray_data->sy = (ray_data->y < ray_data->end_y) * 2 - 1;
	ray_data->err = ray_data->dx - ray_data->dy;
	while (1)
	{
		if (ray_data->x >= mini->offset_x
			&& ray_data->x < mini->offset_x + mini->mini_size
			&& ray_data->y >= mini->offset_y
			&& ray_data->y < mini->offset_y + mini->mini_size)
			mlx_put_pixel(game->mlx.frame.img, ray_data->x,
				ray_data->y, 0x00FF00FF);
		if (ray_data->x == ray_data->end_x && ray_data->y == ray_data->end_y)
			break ;
		advance_ray(ray_data);
	}
}

void	cast_ray_and_draw(t_game *game, t_minimap *mini, int ray)
{
	t_raydraw	ray_data;

	init_ray(game, &ray_data, ray);
	while (!ray_data.hit && ray_data.map_x >= 0
		&& ray_data.map_x < game->map.width
		&& ray_data.map_y >= 0 && ray_data.map_y < game->map.height)
	{
		ray_data.move_x = ray_data.side_x < ray_data.side_y;
		ray_data.side_x += ray_data.move_x * ray_data.delta_x;
		ray_data.side_y += (!ray_data.move_x) * ray_data.delta_y;
		ray_data.map_x += ray_data.move_x * ray_data.step_x;
		ray_data.map_y += (!ray_data.move_x) * ray_data.step_y;
		if (game->map.grid[ray_data.map_y][ray_data.map_x] == '1')
			ray_data.hit = 1;
	}
	draw_ray_line(game, mini, &ray_data);
}
