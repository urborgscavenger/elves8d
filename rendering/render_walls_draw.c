/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls_draw.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 18:35:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:28:48 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"
#include <math.h>

void	perform_dda(t_game *game)
{
	game->ray.hit = 0;
	while (game->ray.hit == 0)
	{
		if (game->ray.sidedist_x < game->ray.sidedist_y)
		{
			game->ray.sidedist_x += game->ray.deltadist_x;
			game->ray.map_x += game->ray.step_x;
			game->ray.side = 0;
		}
		else
		{
			game->ray.sidedist_y += game->ray.deltadist_y;
			game->ray.map_y += game->ray.step_y;
			game->ray.side = 1;
		}
		if (game->ray.map_x < 0 || game->ray.map_x >= game->map.width
			|| game->ray.map_y < 0 || game->ray.map_y >= game->map.height
			|| game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
			game->ray.hit = 1;
	}
}

int	compute_tex_num_and_perp(t_game *game)
{
	int	tex_num;

	if (game->ray.side == 0)
		game->ray.perpwalldist = game->ray.sidedist_x - game->ray.deltadist_x;
	else
		game->ray.perpwalldist = game->ray.sidedist_y - game->ray.deltadist_y;
	if (game->ray.side == 0 && game->ray.step_x > 0)
		tex_num = 2;
	else if (game->ray.side == 0)
		tex_num = 3;
	else if (game->ray.step_y > 0)
		tex_num = 1;
	else
		tex_num = 0;
	return (tex_num);
}

static void	draw_strip_column(t_game *game, int tex_num,
	t_strip *strip, int ray)
{
	int	x;
	int	tex_x;

	tex_x = (int)(strip->wall_x
			* (double)game->tex.textures[tex_num]->width);
	x = ray * STRIP_WIDTH;
	while (x < (ray + 1) * STRIP_WIDTH)
	{
		mlx_put_pixel(game->mlx.frame.img, x, strip->y,
			get_tex_pixel(game->tex.textures[tex_num], tex_x, strip->tex_y));
		x++;
	}
}

static void	init_strip(t_game *game, int tex_num, t_strip *strip)
{
	if (game->ray.side == 0)
		strip->wall_x = game->player.y
			+ game->ray.perpwalldist * game->ray.raydir_y;
	else
		strip->wall_x = game->player.x
			+ game->ray.perpwalldist * game->ray.raydir_x;
	strip->wall_x -= floor(strip->wall_x);
	strip->line_height = (int)(SCREEN_HEIGHT / game->ray.perpwalldist);
	strip->draw_start = -strip->line_height / 2 + SCREEN_HEIGHT / 2;
	if (strip->draw_start < 0)
		strip->draw_start = 0;
	strip->draw_end = strip->line_height / 2 + SCREEN_HEIGHT / 2;
	if (strip->draw_end >= SCREEN_HEIGHT)
		strip->draw_end = SCREEN_HEIGHT - 1;
	strip->step = (double)game->tex.textures[tex_num]->height
		/ strip->line_height;
	strip->tex_pos = (strip->draw_start - SCREEN_HEIGHT / 2
			+ strip->line_height / 2) * strip->step;
	strip->y = strip->draw_start;
}

void	draw_strip(t_game *game, int tex_num, int ray)
{
	t_strip	strip;

	init_strip(game, tex_num, &strip);
	while (strip.y <= strip.draw_end)
	{
		strip.tex_y = (int)strip.tex_pos
			& (game->tex.textures[tex_num]->height - 1);
		strip.tex_pos += strip.step;
		draw_strip_column(game, tex_num, &strip, ray);
		strip.y++;
	}
}
