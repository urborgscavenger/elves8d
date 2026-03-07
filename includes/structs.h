/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 15:12:52 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/07 11:51:21 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

#include <MLX42/MLX42.h>
#include <unistd.h> //read write close
#include <fcntl.h> //open
#include <stdlib.h> //malloc free exit
#include <stdio.h> //printf perror
#include <string.h> // strerror
#include <sys/time.h> //gettimeofday
#include <math.h> //sin cos sqrt etc.

typedef struct s_config
{
    char *no_path;
    char *so_path;
    char *we_path;
    char *ea_path;
    int  floor_color;
    int  ceiling_color;
}   t_config;

typedef struct s_img
{
    mlx_image_t    *img; //mlx_new_image
}   t_img;

typedef struct s_tex //for walls textures
{
    mlx_texture_t *textures[4];
}   t_tex;

typedef struct s_ray
{
    double  raydir_x; //xy - direction of the ray
    double  raydir_y;
    int     map_x; //for DDA (digital differential analyzer)
    int     map_y;
    double  sidedist_x; //distance from current player position to first border (vert o horiz)
    double  sidedist_y;
    double  deltadist_x; //xy how far for the ray to get to next tile of the map
    double  deltadist_y;
    double  perpwalldist; //to avoid fishermans effect - its a real distance to a border (Euclidean distance)
    int     step_x; //shows direction of movement on the map
    int     step_y;
    int     hit; //flag if we hit the border
    int     side; //shows which border we hit (vert o horiz)
}   t_ray;

typedef struct s_player
{
    double  x; //xy - player position
    double  y;
    double  dir_x; //direction of view
    double  dir_y;
    double  plane_x; // field of vision (FOV)
    double  plane_y;
}   t_player;

typedef struct s_map
{
    char    **grid; //2D map table
    int     width;
    int     height;
}   t_map;

typedef struct s_mlx
{
    mlx_t    *mlx; //mlx_init
    t_img   frame;
    int     width;
    int     height;
}   t_mlx;

typedef struct s_game
{
    t_mlx      mlx;
    t_map      map;
    t_player   player;
    t_tex      tex;
    t_config   config;
    t_ray      ray;
}   t_game;

#endif