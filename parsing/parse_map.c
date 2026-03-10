/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 18:19:17 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' '
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int	validate_map_chars(char **grid, int height)
{
	int	i;
	int	j;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (grid[i][j])
		{
			if (!is_valid_map_char(grid[i][j]))
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

static int	set_player_dir(t_player *p, char c, int x, int y)
{
	p->x = x + 0.5;
	p->y = y + 0.5;
	if (c == 'N')
	{
		p->dir_x = 0; p->dir_y = -1;
		p->plane_x = 0.66; p->plane_y = 0;
	}
	else if (c == 'S')
	{
		p->dir_x = 0; p->dir_y = 1;
		p->plane_x = -0.66; p->plane_y = 0;
	}
	else if (c == 'E')
	{
		p->dir_x = 1; p->dir_y = 0;
		p->plane_x = 0; p->plane_y = 0.66;
	}
	else
	{
		p->dir_x = -1; p->dir_y = 0;
		p->plane_x = 0; p->plane_y = -0.66;
	}
	return (0);
}

static int	find_player(t_game *game)
{
	int		i;
	int		j;
	int		found;

	found = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (game->map.grid[i][j])
		{
			if (game->map.grid[i][j] == 'N' || game->map.grid[i][j] == 'S'
				|| game->map.grid[i][j] == 'E' || game->map.grid[i][j] == 'W')
			{
				if (found++)
					return (ft_putstr_fd("Error\nMultiple player starts\n", 2), -1);
				set_player_dir(&game->player, game->map.grid[i][j], j, i);
				game->map.grid[i][j] = '0';
			}
			++j;
		}
		++i;
	}
	if (!found)
		return (ft_putstr_fd("Error\nNo player start position\n", 2), -1);
	return (0);
}

static char	*build_padded_line(const char *line, int target_width)
{
	char	*padded;
	int		len;
	int		i;

	len = (int)ft_strlen(line);
	padded = malloc(target_width + 1);
	if (!padded)
		return (NULL);
	i = 0;
	while (i < len && i < target_width)
	{
		if (line[i] == '\t')
			padded[i] = ' ';
		else
			padded[i] = line[i];
		++i;
	}
	while (i < target_width)
	{
		padded[i] = ' ';
		++i;
	}
	padded[target_width] = '\0';
	return (padded);
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
