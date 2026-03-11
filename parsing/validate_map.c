/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/11 15:16:03 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	run_flood_check(t_map *map, char **copy)
{
	t_flood_params	params;
	int				start_pos[2];

	if (find_floor_start(copy, map->width, map->height, start_pos) != 0)
	{
		ft_putstr_fd("Error\nMap has no open floor cell\n", 2);
		return (-1);
	}
	params.grid = copy;
	params.width = map->width;
	params.height = map->height;
	if (flood_fill(&params, start_pos[0], start_pos[1]) != 0)
	{
		ft_putstr_fd("Error\nMap is not closed/surrounded by walls\n", 2);
		return (-1);
	}
	return (0);
}

int	validate_map(t_map *map)
{
	char	**copy;
	int		ret;

	copy = copy_map(map->grid, map->height, map->width);
	if (!copy)
		return (ft_putstr_fd("Error\nMalloc failed\n", 2), -1);
	ret = run_flood_check(map, copy);
	free_tab(copy);
	return (ret);
}
