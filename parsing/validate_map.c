/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 16:46:30 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static char	**copy_grid(char **grid, int height, int width)
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

static int	flood_fill(char **grid, int x, int y, int w, int h)
{
	if (x < 0 || y < 0 || x >= w || y >= h)
		return (-1);
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return (0);
	if (grid[y][x] == ' ')
		return (-1);
	grid[y][x] = 'V';
	if (flood_fill(grid, x + 1, y, w, h) != 0)
		return (-1);
	if (flood_fill(grid, x - 1, y, w, h) != 0)
		return (-1);
	if (flood_fill(grid, x, y + 1, w, h) != 0)
		return (-1);
	if (flood_fill(grid, x, y - 1, w, h) != 0)
		return (-1);
	return (0);
}

static int	find_start(char **grid, int w, int h, int *sx, int *sy)
{
	int	i;
	int	j;

	i = 0;
	while (i < h)
	{
		j = 0;
		while (j < w)
		{
			if (grid[i][j] == '0')
			{
				*sx = j;
				*sy = i;
				return (0);
			}
			++j;
		}
		++i;
	}
	return (-1);
}

int	validate_map(t_map *map)
{
	char	**copy;
	int		sx;
	int		sy;
	int		ret;

	copy = copy_grid(map->grid, map->height, map->width);
	if (!copy)
		return (ft_putstr_fd("Error\nMalloc failed\n", 2), -1);
	if (find_start(copy, map->width, map->height, &sx, &sy) != 0)
	{
		free_tab(copy);
		return (ft_putstr_fd("Error\nMap has no open floor cell\n", 2), -1);
	}
	ret = flood_fill(copy, sx, sy, map->width, map->height);
	free_tab(copy);
	if (ret != 0)
	{
		ft_putstr_fd("Error\nMap is not closed/surrounded by walls\n", 2);
		return (-1);
	}
	return (0);
}
