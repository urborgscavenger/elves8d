/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 15:21:40 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/16 14:24:38 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# define READ_CHUNK 4096

# include <MLX42/MLX42.h>
# include <unistd.h> //read write close
# include <fcntl.h> //open
# include <stdlib.h> //malloc free exit
# include <stdio.h> //printf perror
# include <string.h> // strerror
# include <sys/time.h> //gettimeofday
# include <math.h> //sin cos sqrt etc.
# include "../includes/structs.h"
# include "../libft/libft.h"

/* read_file.c */
char	*read_file_into_buffer(const char *path, size_t *out_size);
char	**split_lines(const char *buf, size_t size, size_t *out_count);
void	free_lines(char **lines);

/* parse_file.c */
int		parse_file(char *filename, t_game *game);

/* parse_lines.c */
int		parse_lines(char **lines, size_t numlines, t_game *game);

/* parse_config.c */
int		parse_config_line(const char *line, t_game *game);
int		validate_config(t_game *game);

/* parse_colors.c */
int		parse_color(int *dst, const char *line);

/* parse_map.c */
int		parse_map_from_lines(char **lines, size_t start,
			size_t count, t_game *game);

/* parse_player.c */
int		find_player(t_game *game);

/* validate_map.c */
int		validate_map(t_map *map);

/* flood_fill.c */
char	**copy_map(char **grid, int height, int width);
int		find_floor_start(char **grid, int width, int height,
			int *start_pos);
int		flood_fill(t_flood_params *p, int x, int y);

/* utils */
void	free_tab(char **tab);

/* main_utils.c */
void	init_game(t_game *game);
int		load_textures(t_game *game);
int		init_graphics(t_game *game);
void	cleanup_game(t_game *game);

#endif