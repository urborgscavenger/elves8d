/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 12:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 17:52:05 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define NUM_RAYS 40000000
#define MINIMAP_SIZE 100
#define CELL_SIZE 4
#define BUFFER_SIZE 42

void		render_walls(t_game *game);
void		key_hook(mlx_key_data_t keydata, void *param);
uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y);
void		render_minimap(t_game *game);

uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y)
{
	int		index;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	if (x < 0 || x >= (int)tex->width || y < 0 || y >= (int)tex->height)
		return (0x000000FF);
	index = (y * tex->width + x) * 4;
	r = tex->pixels[index];
	g = tex->pixels[index + 1];
	b = tex->pixels[index + 2];
	a = tex->pixels[index + 3];
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

static void	draw_wall_column(t_game *game, int x, int tex_num,
		double wall_x)
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			tex_x;
	int			tex_y;
	int			y;
	uint32_t	color;

	tex_x = (int)(wall_x * game->tex.textures[tex_num]->width);
	line_height = (int)(SCREEN_HEIGHT / game->ray.perpwalldist);
	draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
	if (draw_end >= SCREEN_HEIGHT)
		draw_end = SCREEN_HEIGHT - 1;
	y = draw_start;
	while (y <= draw_end)
	{
		tex_y = (int)((y - draw_start) / (double)line_height
				* game->tex.textures[tex_num]->height);
		color = get_tex_pixel(game->tex.textures[tex_num], tex_x, tex_y);
		mlx_put_pixel(game->mlx.frame.img, x, y, color);
		++y;
	}
}

static int	get_tex_num(t_game *game)
{
	if (game->ray.side == 0)
	{
		if (game->ray.step_x > 0)
			return (2);
		return (3);
	}
	if (game->ray.step_y > 0)
		return (1);
	return (0);
}

static void	dda_step(t_game *game)
{
	while (!game->ray.hit)
	{
		if (game->ray.sidedist_x < game->ray.sidedist_y)
		{
			game->ray.sidedist_x += game->ray.deltadist_x;
			game->ray.map_x += game->ray.step_x;
			game->ray.side = 0;
		}
		else
		{
			game->ray.sidedist_y += game->ray.deltadist_y;
			game->ray.map_y += game->ray.step_y;
			game->ray.side = 1;
		}
		if (game->ray.map_x < 0 || game->ray.map_x >= game->map.width
			|| game->ray.map_y < 0 || game->ray.map_y >= game->map.height
			|| game->map.grid[game->ray.map_y][game->ray.map_x] == '1')
			game->ray.hit = 1;
	}
}

static void	init_ray(t_game *game, int x)
{
	double	camera_x;

	camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
	game->ray.raydir_x = game->player.dir_x + game->player.plane_x * camera_x;
	game->ray.raydir_y = game->player.dir_y + game->player.plane_y * camera_x;
	game->ray.map_x = (int)game->player.x;
	game->ray.map_y = (int)game->player.y;
	game->ray.deltadist_x = fabs(1.0 / game->ray.raydir_x);
	game->ray.deltadist_y = fabs(1.0 / game->ray.raydir_y);
	game->ray.hit = 0;
	if (game->ray.raydir_x < 0)
	{
		game->ray.step_x = -1;
		game->ray.sidedist_x = (game->player.x - game->ray.map_x)
			* game->ray.deltadist_x;
	}
	else
	{
		game->ray.step_x = 1;
		game->ray.sidedist_x = (game->ray.map_x + 1.0 - game->player.x)
			* game->ray.deltadist_x;
	}
	if (game->ray.raydir_y < 0)
	{
		game->ray.step_y = -1;
		game->ray.sidedist_y = (game->player.y - game->ray.map_y)
			* game->ray.deltadist_y;
	}
	else
	{
		game->ray.step_y = 1;
		game->ray.sidedist_y = (game->ray.map_y + 1.0 - game->player.y)
			* game->ray.deltadist_y;
	}
}

void	render_walls(t_game *game)
{
	int		x;
	int		tex_num;
	double	wall_x;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		init_ray(game, x);
		dda_step(game);
		if (game->ray.side == 0)
			game->ray.perpwalldist = game->ray.sidedist_x
				- game->ray.deltadist_x;
		else
			game->ray.perpwalldist = game->ray.sidedist_y
				- game->ray.deltadist_y;
		tex_num = get_tex_num(game);
		if (game->ray.side == 0)
			wall_x = game->player.y + game->ray.perpwalldist
				* game->ray.raydir_y;
		else
			wall_x = game->player.x + game->ray.perpwalldist
				* game->ray.raydir_x;
		wall_x -= floor(wall_x);
		draw_wall_column(game, x, tex_num, wall_x);
		++x;
	}
}

static void	draw_minimap_cell(t_game *game, int i, int j, int cell_size)
{
	int			dy;
	int			dx;
	int			px;
	int			py;
	uint32_t	color;

	if (game->map.grid[i][j] == '1')
		color = 0x000000FF;
	else
		color = 0xFFFFFFFF;
	dy = 0;
	while (dy < cell_size)
	{
		dx = 0;
		while (dx < cell_size)
		{
			px = 10 + j * cell_size + dx;
			py = 10 + i * cell_size + dy;
			if (px < SCREEN_WIDTH && py < SCREEN_HEIGHT)
				mlx_put_pixel(game->mlx.frame.img, px, py, color);
			++dx;
		}
		++dy;
	}
}

void	render_minimap(t_game *game)
{
	int	mini_size;
	int	cell_size;
	int	i;
	int	j;
	int	px;
	int	py;

	mini_size = 200;
	if (game->map.width > game->map.height)
		cell_size = mini_size / game->map.width;
	else
		cell_size = mini_size / game->map.height;
	if (cell_size < 1)
		cell_size = 1;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			draw_minimap_cell(game, i, j, cell_size);
			++j;
		}
		++i;
	}
	px = 10 + (int)(game->player.x * cell_size);
	py = 10 + (int)(game->player.y * cell_size);
	i = -2;
	while (i <= 2)
	{
		j = -2;
		while (j <= 2)
		{
			if (px + j >= 0 && px + j < SCREEN_WIDTH
				&& py + i >= 0 && py + i < SCREEN_HEIGHT)
				mlx_put_pixel(game->mlx.frame.img, px + j, py + i, 0xFF0000FF);
			++j;
		}
		++i;
	}
}

static void	draw_bg(t_game *game)
{
	uint32_t	ceiling_color;
	uint32_t	floor_color;
	int			x;
	int			y;

	ceiling_color = ((uint32_t)game->config.ceiling_color << 8) | 0xFF;
	floor_color = ((uint32_t)game->config.floor_color << 8) | 0xFF;
	y = 0;
	while (y < SCREEN_HEIGHT / 2)
	{
		x = 0;
		while (x < SCREEN_WIDTH)
			mlx_put_pixel(game->mlx.frame.img, x++, y, ceiling_color);
		++y;
	}
	while (y < SCREEN_HEIGHT)
	{
		x = 0;
		while (x < SCREEN_WIDTH)
			mlx_put_pixel(game->mlx.frame.img, x++, y, floor_color);
		++y;
	}
}

static void	redraw(t_game *game)
{
	draw_bg(game);
	render_walls(game);
	render_minimap(game);
}

static void	rotate_player(t_game *game, double angle)
{
	double	cos_a;
	double	sin_a;
	double	old_dir_x;
	double	old_plane_x;

	cos_a = cos(angle);
	sin_a = sin(angle);
	old_dir_x = game->player.dir_x;
	game->player.dir_x = old_dir_x * cos_a - game->player.dir_y * sin_a;
	game->player.dir_y = old_dir_x * sin_a + game->player.dir_y * cos_a;
	old_plane_x = game->player.plane_x;
	game->player.plane_x = old_plane_x * cos_a - game->player.plane_y * sin_a;
	game->player.plane_y = old_plane_x * sin_a + game->player.plane_y * cos_a;
}

static int	try_move(t_game *game, double dx, double dy)
{
	int	nx;
	int	ny;

	nx = (int)(game->player.x + dx);
	ny = (int)(game->player.y + dy);
	if (nx < 0 || nx >= game->map.width || ny < 0 || ny >= game->map.height)
		return (0);
	if (game->map.grid[ny][nx] == '1')
		return (0);
	game->player.x += dx;
	game->player.y += dy;
	return (1);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;
	int		changed;
	double	angle;

	game = (t_game *)param;
	changed = 0;
	angle = M_PI / 10.0;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		return (mlx_close_window(game->mlx.mlx));
	if (keydata.action != MLX_PRESS && keydata.action != MLX_REPEAT)
		return ;
	if (keydata.key == MLX_KEY_W)
		changed = try_move(game, game->player.dir_x, game->player.dir_y);
	else if (keydata.key == MLX_KEY_S)
		changed = try_move(game, -game->player.dir_x, -game->player.dir_y);
	else if (keydata.key == MLX_KEY_A)
		changed = try_move(game, -game->player.plane_x, -game->player.plane_y);
	else if (keydata.key == MLX_KEY_D)
		changed = try_move(game, game->player.plane_x, game->player.plane_y);
	else if (keydata.key == MLX_KEY_LEFT)
	{
		rotate_player(game, -angle);
		changed = 1;
	}
	else if (keydata.key == MLX_KEY_RIGHT)
	{
		rotate_player(game, angle);
		changed = 1;
	}
	if (changed)
		redraw(game);
}

static int	load_textures(t_game *game)
{
	game->tex.textures[0] = mlx_load_png(game->config.no_path);
	game->tex.textures[1] = mlx_load_png(game->config.so_path);
	game->tex.textures[2] = mlx_load_png(game->config.we_path);
	game->tex.textures[3] = mlx_load_png(game->config.ea_path);
	if (!game->tex.textures[0] || !game->tex.textures[1]
		|| !game->tex.textures[2] || !game->tex.textures[3])
		return (ft_putstr_fd("Error\nFailed to load textures\n", 2), -1);
	return (0);
}

static void	cleanup(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (game->tex.textures[i])
			mlx_delete_texture(game->tex.textures[i]);
		++i;
	}
	free_tab(game->map.grid);
	free(game->config.no_path);
	free(game->config.so_path);
	free(game->config.we_path);
	free(game->config.ea_path);
	mlx_terminate(game->mlx.mlx);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (ft_putstr_fd("Error\nUsage: ./cub3D map.cub\n", 2), 1);
	ft_bzero(&game, sizeof(t_game));
	if (parse_file(argv[1], &game) != 0)
		return (1);
	if (load_textures(&game) != 0)
		return (1);
	game.mlx.mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", true);
	if (!game.mlx.mlx)
		return (ft_putstr_fd("Error\nFailed to init MLX42\n", 2), 1);
	mlx_key_hook(game.mlx.mlx, key_hook, &game);
	game.mlx.frame.img = mlx_new_image(game.mlx.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!game.mlx.frame.img)
	{
		mlx_terminate(game.mlx.mlx);
		return (ft_putstr_fd("Error\nFailed to create image\n", 2), 1);
	}
	redraw(&game);
	mlx_image_to_window(game.mlx.mlx, game.mlx.frame.img, 0, 0);
	mlx_loop(game.mlx.mlx);
	cleanup(&game);
	return (0);
}
