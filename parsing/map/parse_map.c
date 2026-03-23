/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/23 21:54:25 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	validate_map_chars(char **grid, int height)
{
	int		i;
	int		j;
	char	c;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (grid[i][j])
		{
			c = grid[i][j];
			if (c != '0' && c != '1' && c != ' '
				&& c != 'N' && c != 'S' && c != 'E' && c != 'W')
			{
				ft_putstr_fd("Error\nInvalid character in map\n", 2);
				return (-1);
			}
			++j;
		}
		++i;
	}
	return (0);
}

static char	*fill_padded_line(char *padded, const char *line, int len, int w)
{
	int	i;

	i = 0;
	while (i < len && i < w)
	{
		if (line[i] == '\t')
			padded[i] = ' ';
		else
			padded[i] = line[i];
		++i;
	}
	while (i < w)
	{
		padded[i] = ' ';
		++i;
	}
	padded[w] = '\0';
	return (padded);
}

static char	*build_padded_line(const char *line, int target_width)
{
	char	*padded;
	int		len;

	padded = malloc(target_width + 1);
	if (!padded)
		return (NULL);
	len = (int)ft_strlen(line);
	return (fill_padded_line(padded, line, len, target_width));
}

static int	compute_map_width(char **lines, size_t start, size_t count)
{
	int		max;
	int		len;
	size_t	i;

	max = 0;
	i = start;
	while (i < start + count)
	{
		len = (int)ft_strlen(lines[i]);
		if (len > max)
			max = len;
		++i;
	}
	return (max);
}

int	parse_map_from_lines(char **lines, size_t start,
		size_t count, t_game *game)
{
	int		width;
	size_t	i;

	width = compute_map_width(lines, start, count);
	game->map.grid = ft_calloc(count + 1, sizeof(char *));
	if (!game->map.grid)
		return (ft_putstr_fd("Error\nMalloc failed\n", 2), -1);
	game->map.width = width;
	game->map.height = (int)count;
	i = 0;
	while (i < count)
	{
		game->map.grid[i] = build_padded_line(lines[start + i], width);
		if (!game->map.grid[i])
			return (ft_putstr_fd("Error\nMalloc failed\n", 2), -1);
		++i;
	}
	game->map.grid[i] = NULL;
	if (validate_map_chars(game->map.grid, game->map.height) != 0)
		return (-1);
	if (find_player(game) != 0)
		return (-1);
	return (0);
}
