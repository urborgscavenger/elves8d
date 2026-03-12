/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 16:50:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:07:16 by mbauer           ###   ########.fr       */
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
# define STRIP_WIDTH (SCREEN_WIDTH / NUM_RAYS)

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
