/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:51:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/11 17:08:57 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>
#include <stdlib.h>

void render_minimap(t_game *game)
{
	int mini_size = 200;
	int cell_size = mini_size / (game->map.width > game->map.height ? game->map.width : game->map.height);
	if (cell_size < 1) cell_size = 1;
	int offset_x = 10, offset_y = 10;

	for (int i = 0; i < game->map.height; i++) {
		for (int j = 0; j < game->map.width; j++) {
			uint32_t color = (game->map.grid[i][j] == '1') ? 0x000000FF : 0xFFFFFF00;
			for (int dy = 0; dy < cell_size; dy++) {
				for (int dx = 0; dx < cell_size; dx++) {
					int px = offset_x + j * cell_size + dx;
					int py = offset_y + i * cell_size + dy;
					if (px < SCREEN_WIDTH && py < SCREEN_HEIGHT) mlx_put_pixel(game->mlx.frame.img, px, py, color);
				}
			}
		}
	}

	int px = offset_x + (int)(game->player.x * cell_size);
	int py = offset_y + (int)(game->player.y * cell_size);
	for (int dy = -2; dy <= 2; dy++) {
		for (int dx = -2; dx <= 2; dx++) {
			int x = px + dx, y = py + dy;
			if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) mlx_put_pixel(game->mlx.frame.img, x, y, 0xFF0000FF);
		}
	}

	for (int ray = 0; ray < NUM_RAYS; ray += 10) {
		double cam_x = 2 * (double)ray / (NUM_RAYS - 1) - 1;
		double ray_x = game->player.dir_x + game->player.plane_x * cam_x;
		double ray_y = game->player.dir_y + game->player.plane_y * cam_x;
		int map_x = (int)game->player.x;
		int map_y = (int)game->player.y;
		double delta_x = fabs(1 / ray_x);
		double delta_y = fabs(1 / ray_y);
		int step_x = (ray_x < 0) ? -1 : 1;
		int step_y = (ray_y < 0) ? -1 : 1;
		double side_x = (ray_x < 0) ? (game->player.x - map_x) * delta_x : (map_x + 1 - game->player.x) * delta_x;
		double side_y = (ray_y < 0) ? (game->player.y - map_y) * delta_y : (map_y + 1 - game->player.y) * delta_y;
		int hit = 0;
		while (!hit && map_x >= 0 && map_x < game->map.width && map_y >= 0 && map_y < game->map.height) {
			if (side_x < side_y) {
				side_x += delta_x;
				map_x += step_x;
			} else {
				side_y += delta_y;
				map_y += step_y;
			}
			if (game->map.grid[map_y][map_x] == '1') hit = 1;
		}
		int end_x = offset_x + map_x * cell_size + cell_size / 2;
		int end_y = offset_y + map_y * cell_size + cell_size / 2;
		int dx = abs(end_x - px), dy = abs(end_y - py);
		int sx = px < end_x ? 1 : -1, sy = py < end_y ? 1 : -1;
		int err = dx - dy;
		while (1) {
			if (px >= offset_x && px < offset_x + mini_size && py >= offset_y && py < offset_y + mini_size) 
				mlx_put_pixel(game->mlx.frame.img, px, py, 0x00FF00FF);
			if (px == end_x && py == end_y) break;
			int e2 = 2 * err;
			if (e2 > -dy) { err -= dy; px += sx; }
			if (e2 < dx) { err += dx; py += sy; }
		}
	}
}
