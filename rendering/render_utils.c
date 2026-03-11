/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:51:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/11 17:08:57 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"

uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y)
{
	if (x < 0 || x >= (int)tex->width || y < 0 || y >= (int)tex->height)
		return 0x000000FF; // Black
	int index = (y * tex->width + x) * 4;
	uint8_t r = tex->pixels[index];
	uint8_t g = tex->pixels[index + 1];
	uint8_t b = tex->pixels[index + 2];
	uint8_t a = tex->pixels[index + 3];
	return (r << 24) | (g << 16) | (b << 8) | a;
}

void draw_background(t_game *game)
{
	uint32_t ceiling_color = (game->config.ceiling_color << 8) | 0xFF;
	uint32_t floor_color = (game->config.floor_color << 8) | 0xFF;
	for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			mlx_put_pixel(game->mlx.frame.img, x, y, ceiling_color);
		}
	}
	for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			mlx_put_pixel(game->mlx.frame.img, x, y, floor_color);
		}
	}
}
