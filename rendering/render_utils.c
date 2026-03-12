/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:18:53 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 17:55:02 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"

static void	fill_rows(t_game *game, int start_y, int end_y, uint32_t color)
{
	int	x;
	int	y;

	y = start_y;
	while (y < end_y)
	{
		x = 0;
		while (x < SCREEN_WIDTH)
		{
			mlx_put_pixel(game->mlx.frame.img, x, y, color);
			x++;
		}
		y++;
	}
}

uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y)
{
	int		index;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	if (x < 0 || x >= (int)tex->width || y < 0 || y >= (int)tex->height)
		return (0x000000FF);
	index = (y * tex->width + x) * 4;
	r = tex->pixels[index];
	g = tex->pixels[index + 1];
	b = tex->pixels[index + 2];
	a = tex->pixels[index + 3];
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

void	draw_background(t_game *game)
{
	uint32_t	ceiling_color;
	uint32_t	floor_color;

	ceiling_color = (game->config.ceiling_color << 8) | 0xFF;
	floor_color = (game->config.floor_color << 8) | 0xFF;
	fill_rows(game, 0, SCREEN_HEIGHT / 2, ceiling_color);
	fill_rows(game, SCREEN_HEIGHT / 2, SCREEN_HEIGHT, floor_color);
}
