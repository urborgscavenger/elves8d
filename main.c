/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 12:00:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:16:17 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include "includes/render.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
	{
		printf("Error\nUsage: ./cub3D map.cub\n");
		return (1);
	}
	init_game(&game);
	if (parse_file(argv[1], &game))
		return (1);
	if (load_textures(&game))
		return (1);
	if (init_graphics(&game))
		return (1);
	mlx_loop(game.mlx.mlx);
	cleanup_game(&game);
	return (0);
}
