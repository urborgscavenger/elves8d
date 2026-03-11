/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 15:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/11 15:01:27 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	connect_flood_fill(t_flood_params *params, int x, int y)
{
	if (x < 0 || y < 0 || x >= params->width || y >= params->height)
		return ;
	if (params->grid[y][x] == '1' || params->grid[y][x] == ' ' ||
		params->grid[y][x] == 'V')
		return ;
	if (params->grid[y][x] == '0')
	{
		params->grid[y][x] = 'V';
		connect_flood_fill(params, x + 1, y);
		connect_flood_fill(params, x - 1, y);
		connect_flood_fill(params, x, y + 1);
		connect_flood_fill(params, x, y - 1);
	}
}

int	find_starting_position(char **grid, int width, int height, int *start_pos)
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

int	check_for_detached_areas(char **grid, int width, int height)
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
