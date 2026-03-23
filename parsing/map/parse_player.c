/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_player.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 15:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/23 21:54:25 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3d.h"

static void	set_ns_dir(t_player *p, char c)
{
	if (c == 'N')
	{
		p->dir_x = 0;
		p->dir_y = -1;
		p->plane_x = 0.66;
		p->plane_y = 0;
	}
	else
	{
		p->dir_x = 0;
		p->dir_y = 1;
		p->plane_x = -0.66;
		p->plane_y = 0;
	}
}

static void	set_ew_dir(t_player *p, char c)
{
	if (c == 'E')
	{
		p->dir_x = 1;
		p->dir_y = 0;
		p->plane_x = 0;
		p->plane_y = 0.66;
	}
	else
	{
		p->dir_x = -1;
		p->dir_y = 0;
		p->plane_x = 0;
		p->plane_y = -0.66;
	}
}

static void	set_player_dir(t_player *p, char c, int x, int y)
{
	p->x = x + 0.5;
	p->y = y + 0.5;
	if (c == 'N' || c == 'S')
		set_ns_dir(p, c);
	else
		set_ew_dir(p, c);
}

static int	process_player_cell(t_game *game, int i, int j, int *found)
{
	char	c;

	c = game->map.grid[i][j];
	if (c != 'N' && c != 'S' && c != 'E' && c != 'W')
		return (0);
	if ((*found)++)
	{
		ft_putstr_fd("Error\nMultiple player starts\n", 2);
		return (-1);
	}
	set_player_dir(&game->player, c, j, i);
	game->map.grid[i][j] = '0';
	return (0);
}

int	find_player(t_game *game)
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
			if (process_player_cell(game, i, j, &found) != 0)
				return (-1);
			++j;
		}
		++i;
	}
	if (!found)
		return (ft_putstr_fd("Error\nNo player start position\n", 2), -1);
	return (0);
}
