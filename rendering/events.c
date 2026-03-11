/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:51:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/11 17:08:57 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"

void move_player(t_game *game, double dx, double dy)
{
	int new_x = (int)(game->player.x + dx);
	int new_y = (int)(game->player.y + dy);
	if (new_x >= 0 && new_x < game->map.width && new_y >= 0 && new_y < game->map.height &&
		game->map.grid[new_y][new_x] != '1') {
		game->player.x += dx;
		game->player.y += dy;
	}
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game *game = (t_game *)param;
	int changed = 0;

	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(game->mlx.mlx);
		return;
	}

	if (keydata.action != MLX_PRESS) return;

	if (keydata.key == MLX_KEY_W)
	{
		move_player(game, game->player.dir_x, game->player.dir_y);
		changed = 1;
	}
	else if (keydata.key == MLX_KEY_S)
	{
		move_player(game, -game->player.dir_x, -game->player.dir_y);
		changed = 1;
	}
	else if (keydata.key == MLX_KEY_A)
	{
		move_player(game, -game->player.plane_x, -game->player.plane_y);
		changed = 1;
	}
	else if (keydata.key == MLX_KEY_D)
	{
		move_player(game, game->player.plane_x, game->player.plane_y);
		changed = 1;
	}
	else if (keydata.key == MLX_KEY_LEFT)
	{
		double angle = 3.141592653589793 / 10.0;
		double cos_a = cos(-angle);
		double sin_a = sin(-angle);
		double old_dir_x = game->player.dir_x;
		game->player.dir_x = old_dir_x * cos_a - game->player.dir_y * sin_a;
		game->player.dir_y = old_dir_x * sin_a + game->player.dir_y * cos_a;
		double old_plane_x = game->player.plane_x;
		game->player.plane_x = old_plane_x * cos_a - game->player.plane_y * sin_a;
		game->player.plane_y = old_plane_x * sin_a + game->player.plane_y * cos_a;
		changed = 1;
	}
	else if (keydata.key == MLX_KEY_RIGHT)
	{
		double angle = 3.141592653589793 / 10.0;
		double cos_a = cos(angle);
		double sin_a = sin(angle);
		double old_dir_x = game->player.dir_x;
		game->player.dir_x = old_dir_x * cos_a - game->player.dir_y * sin_a;
		game->player.dir_y = old_dir_x * sin_a + game->player.dir_y * cos_a;
		double old_plane_x = game->player.plane_x;
		game->player.plane_x = old_plane_x * cos_a - game->player.plane_y * sin_a;
		game->player.plane_y = old_plane_x * sin_a + game->player.plane_y * cos_a;
		changed = 1;
	}

	if (changed)
	{
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				mlx_put_pixel(game->mlx.frame.img, x, y, 0x000000FF);
			}
		}
		draw_background(game);
		render_walls(game);
		render_minimap(game);
	}
}
