/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/23 21:54:25 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static int	check_extension(const char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if (len < 5)
		return (ft_putstr_fd("Error\nFile must have .cub extension\n", 2), -1);
	if (ft_strncmp(filename + len - 4, ".cub", 4) != 0)
		return (ft_putstr_fd("Error\nFile must have .cub extension\n", 2), -1);
	return (0);
}

static void	init_config(t_config *cfg)
{
	cfg->no_path = NULL;
	cfg->so_path = NULL;
	cfg->we_path = NULL;
	cfg->ea_path = NULL;
	cfg->floor_color = -1;
	cfg->ceiling_color = -1;
}

int	parse_file(char *filename, t_game *game)
{
	char	*buf;
	char	**lines;
	size_t	buf_size;
	size_t	lines_count;
	int		ret;

	if (!filename || !game)
		return (ft_putstr_fd("Error\nInvalid arguments\n", 2), -1);
	if (check_extension(filename) != 0)
		return (-1);
	init_config(&game->config);
	buf = read_file_into_buffer(filename, &buf_size);
	if (!buf)
		return (-1);
	lines = split_lines(buf, buf_size, &lines_count);
	free(buf);
	if (!lines)
		return (-1);
	ret = parse_lines(lines, lines_count, game);
	free_lines(lines);
	return (ret);
}
