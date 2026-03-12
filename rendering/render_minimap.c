/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:18:41 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:03:36 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"

static void	draw_cell(t_game *game, t_minimap *mini, int map_x, int map_y)
{
	int		dx;
	int		dy;
	int		px;
	int		py;
	int		color;

	color = 0xFFFFFF00;
	if (game->map.grid[map_y][map_x] == '1')
		color = 0x000000FF;
	py = mini->offset_y + map_y * mini->cell_size;
	dy = 0;
	while (dy < mini->cell_size)
	{
		px = mini->offset_x + map_x * mini->cell_size;
		dx = 0;
		while (dx < mini->cell_size)
		{
			if (px + dx < SCREEN_WIDTH && py + dy < SCREEN_HEIGHT)
				mlx_put_pixel(game->mlx.frame.img, px + dx, py + dy, color);
			dx++;
		}
		dy++;
	}
}

static void	draw_minimap_base(t_game *game, t_minimap *mini)
{
	int		x;
	int		y;
	int		dx;
	int		dy;

	y = -1;
	while (++y < game->map.height)
	{
		x = -1;
		while (++x < game->map.width)
			draw_cell(game, mini, x, y);
	}
	dy = -3;
	while (++dy <= 2)
	{
		dx = -3;
		while (++dx <= 2)
			if (mini->px + dx >= 0 && mini->px + dx < SCREEN_WIDTH
				&& mini->py + dy >= 0 && mini->py + dy < SCREEN_HEIGHT)
				mlx_put_pixel(game->mlx.frame.img, mini->px + dx,
					mini->py + dy, 0xFF0000FF);
	}
}

void	render_minimap(t_game *game)
{
	t_minimap	mini;
	int			ray;

	mini.mini_size = 200;
	mini.cell_size = mini.mini_size / game->map.width;
	if (game->map.height > game->map.width)
		mini.cell_size = mini.mini_size / game->map.height;
	if (mini.cell_size < 1)
		mini.cell_size = 1;
	mini.offset_x = 10;
	mini.offset_y = 10;
	mini.px = mini.offset_x + (int)(game->player.x * mini.cell_size);
	mini.py = mini.offset_y + (int)(game->player.y * mini.cell_size);
	draw_minimap_base(game, &mini);
	ray = 0;
	while (ray < NUM_RAYS)
	{
		cast_ray_and_draw(game, &mini, ray);
		ray += 10;
	}
}
