/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 15:12:52 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/11 14:59:12 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <MLX42/MLX42.h>

typedef struct s_config
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor_color;
	int		ceiling_color;
}			t_config;

typedef struct s_img
{
	mlx_image_t	*img;
}			t_img;

typedef struct s_tex
{
	mlx_texture_t	*textures[4];
}			t_tex;

typedef struct s_ray
{
	double	raydir_x;
	double	raydir_y;
	int		map_x;
	int		map_y;
	double	sidedist_x;
	double	sidedist_y;
	double	deltadist_x;
	double	deltadist_y;
	double	perpwalldist;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
}			t_ray;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}			t_player;

typedef struct s_map
{
	char	**grid;
	int		width;
	int		height;
}			t_map;

typedef struct s_flood_params
{
	char	**grid;
	int		width;
	int		height;
}			t_flood_params;

typedef struct s_mlx
{
	mlx_t	*mlx;
	t_img	frame;
	int		width;
	int		height;
}			t_mlx;

typedef struct s_game
{
	t_mlx		mlx;
	t_map		map;
	t_player	player;
	t_tex		tex;
	t_config	config;
	t_ray		ray;
}			t_game;

#endif