/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:18:59 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/11 22:23:26 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>

void	render_walls(t_game *game)
{
	for (int ray = 0; ray < NUM_RAYS; ray++) {
		// Calculate ray position and direction
		double camera_x = 2 * ray / (double)NUM_RAYS - 1;
		game->ray.raydir_x = game->player.dir_x + game->player.plane_x * camera_x;
		game->ray.raydir_y = game->player.dir_y + game->player.plane_y * camera_x;

		game->ray.map_x = (int)game->player.x;
		game->ray.map_y = (int)game->player.y;

		// Length of ray from one x or y-side to next x or y-side
		game->ray.deltadist_x = fabs(1 / game->ray.raydir_x);
		game->ray.deltadist_y = fabs(1 / game->ray.raydir_y);

		// Calculate step and initial sideDist
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

		// Perform DDA
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
			
			// Check bounds and hit
			if (game->ray.map_x < 0 || game->ray.map_x >= game->map.width ||
				game->ray.map_y < 0 || game->ray.map_y >= game->map.height ||
				game->map.grid[game->ray.map_y][game->ray.map_x] == '1') {
				game->ray.hit = 1;
			}
		}

		// Calculate distance projected on camera direction
		if (game->ray.side == 0)
			game->ray.perpwalldist = game->ray.sidedist_x - game->ray.deltadist_x;
		else
			game->ray.perpwalldist = game->ray.sidedist_y - game->ray.deltadist_y;

		// Get texture index based on cardinal direction
		int tex_num;
		if (game->ray.side == 0)
			tex_num = (game->ray.step_x > 0) ? 2 : 3; // East / West
		else
			tex_num = (game->ray.step_y > 0) ? 1 : 0; // South / North

		// Calculate exactly where the wall was hit
		double wall_x;
		if (game->ray.side == 0)
			wall_x = game->player.y + game->ray.perpwalldist * game->ray.raydir_y;
		else
			wall_x = game->player.x + game->ray.perpwalldist * game->ray.raydir_x;
		wall_x -= floor(wall_x);

		// X coordinate on the texture
		int tex_width = game->tex.textures[tex_num]->width;
		int tex_x = (int)(wall_x * tex_width);
		
		// Fix texture flipping on specific sides
		if ((game->ray.side == 0 && game->ray.raydir_x > 0) ||
			(game->ray.side == 1 && game->ray.raydir_y < 0)) {
			tex_x = tex_width - tex_x - 1;
		}

		// Calculate height of line to draw on screen
		int line_height = (int)(SCREEN_HEIGHT / game->ray.perpwalldist);

		// Calculate lowest and highest pixel to fill in current stripe
		int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0) draw_start = 0;
		int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;

		// Calculate strip bounds (for performance/scaling)
		int x_start = ray * STRIP_WIDTH;
		int x_end = (ray + 1) * STRIP_WIDTH;
		if (ray == NUM_RAYS - 1) x_end = SCREEN_WIDTH;

		// Step variable for texture mapping performance
		double step = 1.0 * game->tex.textures[tex_num]->height / line_height;
		double tex_pos = (draw_start - SCREEN_HEIGHT / 2 + line_height / 2) * step;

		// Draw the vertical strip
		for (int y = draw_start; y <= draw_end; y++) {
			int tex_y = (int)tex_pos & (game->tex.textures[tex_num]->height - 1);
			tex_pos += step;
			
			uint32_t color = get_tex_pixel(game->tex.textures[tex_num], tex_x, tex_y);
			for (int x = x_start; x < x_end; x++) {
				mlx_put_pixel(game->mlx.frame.img, x, y, color);
			}
		}
	}
}