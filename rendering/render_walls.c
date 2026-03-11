/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:51:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/11 17:08:57 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>

void	render_walls(t_game *game)
{
	for (int ray = 0; ray < NUM_RAYS; ray++) {
		int x_start = ray * STRIP_WIDTH;
		int x_end = (ray + 1) * STRIP_WIDTH;
		if (ray == NUM_RAYS - 1) x_end = SCREEN_WIDTH;

		double camera_x = 2 * ray / (double)NUM_RAYS - 1;
		game->ray.raydir_x = game->player.dir_x + game->player.plane_x * camera_x;
		game->ray.raydir_y = game->player.dir_y + game->player.plane_y * camera_x;

		game->ray.map_x = (int)game->player.x;
		game->ray.map_y = (int)game->player.y;

		game->ray.deltadist_x = fabs(1 / game->ray.raydir_x);
		game->ray.deltadist_y = fabs(1 / game->ray.raydir_y);

		if (game->ray.raydir_x < 0) {
			game->ray.step_x = -1;
			game->ray.sidedist_x = (game->player.x - game->ray.map_x) * game->ray.deltadist_x;
		} else {
			game->ray.step_x = 1;
			game->ray.sidedist_x = (game->ray.map_x + 1.0 - game->player.x) * game->ray.deltadist_x;
		}
		if (game->ray.raydir_y < 0) {
			game->ray.step_y = -1;
			game->ray.sidedist_y = (game->player.y - game->ray.map_y) * game->ray.deltadist_y;
		} else {
			game->ray.step_y = 1;
			game->ray.sidedist_y = (game->ray.map_y + 1.0 - game->player.y) * game->ray.deltadist_y;
		}

		game->ray.hit = 0;
		while (!game->ray.hit) {
			if (game->ray.sidedist_x < game->ray.sidedist_y) {
				game->ray.sidedist_x += game->ray.deltadist_x;
				game->ray.map_x += game->ray.step_x;
				game->ray.side = 0;
			} else {
				game->ray.sidedist_y += game->ray.deltadist_y;
				game->ray.map_y += game->ray.step_y;
				game->ray.side = 1;
			}
			if (game->ray.map_x < 0 || game->ray.map_x >= game->map.width ||
				game->ray.map_y < 0 || game->ray.map_y >= game->map.height ||
				game->map.grid[game->ray.map_y][game->ray.map_x] == '1') {
				game->ray.hit = 1;
			}
		}

		if (game->ray.side == 0)
			game->ray.perpwalldist = game->ray.sidedist_x - game->ray.deltadist_x;
		else
			game->ray.perpwalldist = game->ray.sidedist_y - game->ray.deltadist_y;

		int tex_num = 0;
		if (game->ray.side == 0) {
			if (game->ray.step_x > 0) tex_num = 2;
			else tex_num = 3;
		} else {
			if (game->ray.step_y > 0) tex_num = 1;
			else tex_num = 0;
		}

		double wall_x;
		if (game->ray.side == 0) wall_x = game->player.y + game->ray.perpwalldist * game->ray.raydir_y;
		else wall_x = game->player.x + game->ray.perpwalldist * game->ray.raydir_x;
		wall_x -= floor(wall_x);
		int tex_x = (int)(wall_x * game->tex.textures[tex_num]->width);

		int line_height = (int)(SCREEN_HEIGHT / game->ray.perpwalldist);
		int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0) draw_start = 0;
		int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;

		for (int x = x_start; x < x_end; x++) {
			for (int y = draw_start; y <= draw_end; y++) {
				int tex_y = (int)((y - draw_start) / (double)line_height * game->tex.textures[tex_num]->height);
				uint32_t color = get_tex_pixel(game->tex.textures[tex_num], tex_x, tex_y);
				mlx_put_pixel(game->mlx.frame.img, x, y, color);
			}
		}
	}
}
