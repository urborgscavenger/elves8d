/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 19:25:41 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 19:25:44 by wilisson         ###   ########.fr       */
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
