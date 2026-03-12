/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 19:00:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 19:01:48 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"

static void	rotate_player(t_game *game, double angle)
{
	double	cos_a;
	double	sin_a;
	double	old_dir_x;
	double	old_plane_x;

	cos_a = cos(angle);
	sin_a = sin(angle);
	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	game->player.dir_x = old_dir_x * cos_a - game->player.dir_y * sin_a;
	game->player.dir_y = old_dir_x * sin_a + game->player.dir_y * cos_a;
	game->player.plane_x = old_plane_x * cos_a - game->player.plane_y * sin_a;
	game->player.plane_y = old_plane_x * sin_a + game->player.plane_y * cos_a;
}

static void	redraw_scene(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < SCREEN_HEIGHT)
	{
		x = 0;
		while (x < SCREEN_WIDTH)
		{
			mlx_put_pixel(game->mlx.frame.img, x, y, 0x000000FF);
			x++;
		}
		y++;
	}
	draw_background(game);
	render_walls(game);
	render_minimap(game);
}

static int	handle_keypress(t_game *game, keys_t key)
{
	if (key == MLX_KEY_W)
		move_player(game, game->player.dir_x, game->player.dir_y);
	else if (key == MLX_KEY_S)
		move_player(game, -game->player.dir_x, -game->player.dir_y);
	else if (key == MLX_KEY_A)
		move_player(game, -game->player.plane_x, -game->player.plane_y);
	else if (key == MLX_KEY_D)
		move_player(game, game->player.plane_x, game->player.plane_y);
	else if (key == MLX_KEY_LEFT)
		rotate_player(game, -3.141592653589793 / 4.0);
	else if (key == MLX_KEY_RIGHT)
		rotate_player(game, 3.141592653589793 / 4.0);
	else
		return (0);
	return (1);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(game->mlx.mlx);
		return ;
	}
	if (keydata.action != MLX_PRESS)
		return ;
	if (handle_keypress(game, keydata.key))
		redraw_scene(game);
}
