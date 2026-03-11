/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_connectivity.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/11 15:16:03 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	check_for_detached_areas(char **grid, int width, int height)
{
	int	i;
	int	j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (grid[i][j] == '0')
				return (-1);
			++j;
		}
		++i;
	}
	return (0);
}

static int	perform_connectivity_check(t_map *map, char **copy)
{
	t_flood_params	params;
	int				start_pos[2];

	if (find_floor_start(copy, map->width, map->height, start_pos) != 0)
	{
		ft_putstr_fd("Error\nNo open floor position found in map\n", 2);
		return (-1);
	}
	params.grid = copy;
	params.width = map->width;
	params.height = map->height;
	flood_fill(&params, start_pos[0], start_pos[1]);
	if (check_for_detached_areas(copy, map->width, map->height) != 0)
	{
		ft_putstr_fd("Error\nMap has detached areas unreachable", 2);
		ft_putstr_fd(" from starting position\n", 2);
		return (-1);
	}
	return (0);
}

int	validate_map_connectivity(t_map *map)
{
	char	**copy;
	int		ret;

	copy = copy_map(map->grid, map->height, map->width);
	if (!copy)
	{
		ft_putstr_fd("Error\nMalloc failed during connectivity check\n", 2);
		return (-1);
	}
	ret = perform_connectivity_check(map, copy);
	free_tab(copy);
	return (ret);
}
