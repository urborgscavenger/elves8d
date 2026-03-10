/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 18:23:07 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	is_valid_map_char(char c)
{
	return (c == '0' || c == '1' || c == ' ' || c == '\t'
		|| c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

static int	is_map_line(const char *s)
{
	size_t	i;

	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	while (s[i])
	{
		if (!is_valid_map_char(s[i]))
			return (0);
		++i;
	}
	i = 0;
	while (s[i] == ' ' || s[i] == '\t')
		++i;
	return (s[i] != '\0');
}

static int	parse_config_section(char **lines, size_t numlines,
		size_t *out_i, t_game *game)
{
	size_t	i;

	i = 0;
	while (i < numlines && !is_map_line(lines[i]))
	{
		if (lines[i][0] != '\0')
		{
			if (parse_config_line(lines[i], game) != 0)
				return (-1);
		}
		++i;
	}
	*out_i = i;
	return (0);
}

static size_t	find_map_end(char **lines, size_t start, size_t numlines)
{
	size_t	end;

	end = start;
	while (end < numlines)
	{
		if (!is_map_line(lines[end]) && lines[end][0] == '\0')
			break ;
		++end;
	}
	return (end);
}

int	parse_lines(char **lines, size_t numlines, t_game *game)
{
	size_t	i;
	size_t	map_end;

	if (!lines || numlines == 0 || !game)
		return (ft_putstr_fd("Error\nInvalid arguments\n", 2), -1);
	if (parse_config_section(lines, numlines, &i, game) != 0)
		return (-1);
	if (i >= numlines)
		return (ft_putstr_fd("Error\nNo map found\n", 2), -1);
	map_end = find_map_end(lines, i, numlines);
	if (parse_map_from_lines(lines, i, map_end - i, game) != 0)
		return (-1);
	if (validate_config(game) != 0)
		return (-1);
	if (validate_map(&game->map) != 0)
		return (-1);
	if (validate_map_connectivity(&game->map) != 0)
		return (-1);
	return (0);
}
