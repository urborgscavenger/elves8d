/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 16:45:26 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	parse_texture(char **dst, const char *line)
{
	char	*path;

	if (*dst)
	{
		ft_putstr_fd("Error\nDuplicate texture identifier\n", 2);
		return (-1);
	}
	while (*line == ' ' || *line == '\t')
		++line;
	path = ft_strtrim(line, " \t\r\n");
	if (!path || path[0] == '\0')
	{
		free(path);
		ft_putstr_fd("Error\nEmpty texture path\n", 2);
		return (-1);
	}
	*dst = path;
	return (0);
}

static int	validate_color_value(int val, const char *comp)
{
	int	i;

	i = 0;
	while (comp[i] == ' ' || comp[i] == '\t')
		++i;
	while (comp[i] >= '0' && comp[i] <= '9')
		++i;
	while (comp[i] == ' ' || comp[i] == '\t')
		++i;
	if (comp[i] != '\0' && comp[i] != '\r')
	{
		ft_putstr_fd("Error\nInvalid color format\n", 2);
		return (-1);
	}
	if (val < 0 || val > 255)
	{
		ft_putstr_fd("Error\nColor value out of range [0,255]\n", 2);
		return (-1);
	}
	return (0);
}

static int	parse_color_value(const char *str)
{
	char	**rgb;
	int		r;
	int		g;
	int		b;

	rgb = ft_split(str, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2] || rgb[3])
	{
		ft_putstr_fd("Error\nInvalid color format (expected R,G,B)\n", 2);
		if (rgb)
			free_tab(rgb);
		return (-1);
	}
	r = ft_atoi(rgb[0]);
	g = ft_atoi(rgb[1]);
	b = ft_atoi(rgb[2]);
	if (validate_color_value(r, rgb[0]) || validate_color_value(g, rgb[1])
		|| validate_color_value(b, rgb[2]))
	{
		free_tab(rgb);
		return (-1);
	}
	free_tab(rgb);
	return ((r << 16) | (g << 8) | b);
}

static int	parse_color(int *dst, const char *line)
{
	int		result;
	char	*trimmed;

	if (*dst != -1)
	{
		ft_putstr_fd("Error\nDuplicate color identifier\n", 2);
		return (-1);
	}
	while (*line == ' ' || *line == '\t')
		++line;
	trimmed = ft_strtrim(line, " \t\r\n");
	if (!trimmed || trimmed[0] == '\0')
	{
		free(trimmed);
		ft_putstr_fd("Error\nEmpty color value\n", 2);
		return (-1);
	}
	result = parse_color_value(trimmed);
	free(trimmed);
	if (result < 0)
		return (-1);
	*dst = result;
	return (0);
}

int	parse_config_line(const char *line, t_game *game)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		return (parse_texture(&game->config.no_path, line + 3));
	if (ft_strncmp(line, "SO ", 3) == 0)
		return (parse_texture(&game->config.so_path, line + 3));
	if (ft_strncmp(line, "WE ", 3) == 0)
		return (parse_texture(&game->config.we_path, line + 3));
	if (ft_strncmp(line, "EA ", 3) == 0)
		return (parse_texture(&game->config.ea_path, line + 3));
	if (line[0] == 'F' && (line[1] == ' ' || line[1] == '\t'))
		return (parse_color(&game->config.floor_color, line + 2));
	if (line[0] == 'C' && (line[1] == ' ' || line[1] == '\t'))
		return (parse_color(&game->config.ceiling_color, line + 2));
	ft_putstr_fd("Error\nUnknown identifier: ", 2);
	ft_putstr_fd((char *)line, 2);
	ft_putstr_fd("\n", 2);
	return (-1);
}

int	validate_config(t_game *game)
{
	if (!game->config.no_path)
		return (ft_putstr_fd("Error\nMissing NO texture\n", 2), -1);
	if (!game->config.so_path)
		return (ft_putstr_fd("Error\nMissing SO texture\n", 2), -1);
	if (!game->config.we_path)
		return (ft_putstr_fd("Error\nMissing WE texture\n", 2), -1);
	if (!game->config.ea_path)
		return (ft_putstr_fd("Error\nMissing EA texture\n", 2), -1);
	if (game->config.floor_color == -1)
		return (ft_putstr_fd("Error\nMissing F color\n", 2), -1);
	if (game->config.ceiling_color == -1)
		return (ft_putstr_fd("Error\nMissing C color\n", 2), -1);
	return (0);
}
