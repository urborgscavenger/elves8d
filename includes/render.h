/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:50:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:28:54 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDER_H
# define RENDER_H

# include "cub3d.h"
# include <stdint.h>
# include <math.h>

# define SCREEN_WIDTH 1024
# define SCREEN_HEIGHT 768
# define NUM_RAYS 256
# define MINIMAP_SIZE 100
# define CELL_SIZE 4
# define BUFFER_SIZE 42
# define STRIP_WIDTH 8

typedef struct s_strip
{
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		y;
	int		tex_y;
	double	wall_x;
	double	step;
	double	tex_pos;
}t_strip;

typedef struct s_raydraw
{
	double	cam_x;
	double	ray_x;
	double	ray_y;
	double	delta_x;
	double	delta_y;
	double	side_x;
	double	side_y;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		end_x;
	int		end_y;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	int		x;
	int		y;
	int		move_x;
	int		hit;
}	t_raydraw;

typedef struct s_minimap
{
	int	mini_size;
	int	cell_size;
	int	offset_x;
	int	offset_y;
	int	px;
	int	py;
}t_minimap;

void		render_walls(t_game *game);
void		setup_ray(t_game *game, int ray);
void		perform_dda(t_game *game);
int			compute_tex_num_and_perp(t_game *game);
void		draw_strip(t_game *game, int tex_num, int ray);

uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y);
void		draw_background(t_game *game);

void		render_minimap(t_game *game);
void		cast_ray_and_draw(t_game *game, t_minimap *mini, int ray);

void		move_player(t_game *game, double dx, double dy);
void		key_hook(mlx_key_data_t keydata, void *param);

#endif
