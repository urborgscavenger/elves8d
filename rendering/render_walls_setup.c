/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls_setup.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 18:35:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:07:16 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>

static void	setup_ray_x(t_game *game)
{
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
}

static void	setup_ray_y(t_game *game)
{
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

void	setup_ray(t_game *game, int ray)
{
	double	camera_x;

	camera_x = 2.0 * ray / (double)NUM_RAYS - 1.0;
	game->ray.raydir_x = game->player.dir_x + game->player.plane_x * camera_x;
	game->ray.raydir_y = game->player.dir_y + game->player.plane_y * camera_x;
	game->ray.map_x = (int)game->player.x;
	game->ray.map_y = (int)game->player.y;
	game->ray.deltadist_x = fabs(1.0 / game->ray.raydir_x);
	game->ray.deltadist_y = fabs(1.0 / game->ray.raydir_y);
	setup_ray_x(game);
	setup_ray_y(game);
}
