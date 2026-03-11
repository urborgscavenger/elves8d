/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:25:49 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 19:26:15 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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

int	parse_color(int *dst, const char *line)
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
