/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 15:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/11 15:16:03 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	**copy_map(char **grid, int height, int width)
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

int	find_floor_start(char **grid, int width, int height, int *start_pos)
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
				start_pos[0] = j;
				start_pos[1] = i;
				return (0);
			}
			++j;
		}
		++i;
	}
	return (-1);
}

int	flood_fill(t_flood_params *p, int x, int y)
{
	int	ret;

	if (x < 0 || y < 0 || x >= p->width || y >= p->height)
		return (-1);
	if (p->grid[y][x] == '1' || p->grid[y][x] == 'V')
		return (0);
	if (p->grid[y][x] == ' ')
		return (-1);
	p->grid[y][x] = 'V';
	ret = 0;
	if (flood_fill(p, x + 1, y) != 0)
		ret = -1;
	if (flood_fill(p, x - 1, y) != 0)
		ret = -1;
	if (flood_fill(p, x, y + 1) != 0)
		ret = -1;
	if (flood_fill(p, x, y - 1) != 0)
		ret = -1;
	return (ret);
}
