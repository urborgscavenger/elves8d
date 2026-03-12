/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 17:18:59 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/12 18:07:16 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/render.h"

void	render_walls(t_game *game)
{
	int	ray;
	int	tex_num;

	ray = 0;
	while (ray < NUM_RAYS)
	{
		setup_ray(game, ray);
		perform_dda(game);
		tex_num = compute_tex_num_and_perp(game);
		draw_strip(game, tex_num, ray);
		ray++;
	}
}
