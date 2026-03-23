/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/23 21:54:25 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	handle_floor_cell(t_flood_params *params, int x, int y,
			int *found_floor)
{
	*found_floor = 1;
	if (flood_fill(params, x, y) != 0)
	{
		ft_putstr_fd("Error\nMap is not closed/", 2);
		ft_putstr_fd("surrounded by walls\n", 2);
		return (-1);
	}
	return (0);
}

static int	scan_grid(t_flood_params *params, char **copy, int *found_floor)
{
	int			i;
	int			j;

	i = 0;
	while (i < params->height)
	{
		j = 0;
		while (j < params->width)
		{
			if (copy[i][j] == '0')
			{
				if (handle_floor_cell(params, j, i, found_floor) != 0)
					return (-1);
			}
			++j;
		}
		++i;
	}
	return (0);
}

static int	run_flood_check(t_map *map, char **copy)
{
	t_flood_params	params;
	int				found_floor;

	params.grid = copy;
	params.width = map->width;
	params.height = map->height;
	found_floor = 0;
	if (scan_grid(&params, copy, &found_floor) != 0)
		return (-1);
	if (!found_floor)
	{
		ft_putstr_fd("Error\nMap has no open ", 2);
		ft_putstr_fd("floor cell\n", 2);
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
