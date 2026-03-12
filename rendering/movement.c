/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:19:04 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 19:01:48 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"

static int	is_walkable_tile(t_game *game, int x, int y)
{
	if (x < 0 || x >= game->map.width || y < 0 || y >= game->map.height)
		return (0);
	if (game->map.grid[y][x] != '0')
		return (0);
	return (1);
}

void	move_player(t_game *game, double dx, double dy)
{
	int	new_x;
	int	new_y;

	new_x = (int)(game->player.x + dx);
	new_y = (int)(game->player.y + dy);
	if (is_walkable_tile(game, new_x, (int)game->player.y))
		game->player.x += dx;
	if (is_walkable_tile(game, (int)game->player.x, new_y))
		game->player.y += dy;
}
