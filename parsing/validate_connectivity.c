/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_connectivity.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 17:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 18:32:01 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static char	**copy_map_for_connectivity(char **grid, int height, int width)
{
	char	**copy;
	int		i;

	copy = ft_calloc(height + 1, sizeof(char *));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < height)
	{
		copy[i] = ft_calloc(width + 1, sizeof(char));
		if (!copy[i])
			return (free_tab(copy), NULL);
		ft_memcpy(copy[i], grid[i], width);
		copy[i][width] = '\0';
		++i;
	}
	copy[i] = NULL;
	return (copy);
}

static void	connectivity_flood_fill(char **grid, int x, int y, int width, int height)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return ;
	if (grid[y][x] == '1' || grid[y][x] == ' ' || grid[y][x] == 'V')
		return ;
	if (grid[y][x] == '0')
	{
		grid[y][x] = 'V';
		connectivity_flood_fill(grid, x + 1, y, width, height);
		connectivity_flood_fill(grid, x - 1, y, width, height);
		connectivity_flood_fill(grid, x, y + 1, width, height);
		connectivity_flood_fill(grid, x, y - 1, width, height);
	}
}

static int	find_starting_position(char **grid, int width, int height, int *px, int *py)
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
			{
				*px = j;
				*py = i;
				return (0);
			}
			++j;
		}
		++i;
	}
	return (-1);
}

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
			{
				return (-1);
			}
			++j;
		}
		++i;
	}
	return (0);
}

int	validate_map_connectivity(t_map *map)
{
	char	**copy;
	int		start_x;
	int		start_y;
	int		ret;

	copy = copy_map_for_connectivity(map->grid, map->height, map->width);
	if (!copy)
		return (ft_putstr_fd("Error\nMalloc failed during connectivity check\n", 2), -1);
	
	if (find_starting_position(copy, map->width, map->height, &start_x, &start_y) != 0)
	{
		free_tab(copy);
		return (ft_putstr_fd("Error\nNo open floor position found in map\n", 2), -1);
	}
	
	connectivity_flood_fill(copy, start_x, start_y, map->width, map->height);
	
	ret = check_for_detached_areas(copy, map->width, map->height);
	free_tab(copy);
	
	if (ret != 0)
	{
		ft_putstr_fd("Error\nMap has detached areas unreachable from starting position\n", 2);
		return (-1);
	}
	return (0);
}
