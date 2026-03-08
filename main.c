/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 12:00:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/08 01:13:27 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define MINIMAP_SIZE 100
#define CELL_SIZE 4
#define BUFFER_SIZE 42

void	render_walls(t_game *game);
void	key_hook(mlx_key_data_t keydata, void *param);
uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
void	free_split(char **arr);

char *get_next_line(int fd)
{
	static char b[BUFFER_SIZE];
	static int pos = 0;
	static int rd = 0;

	if(pos >= rd)
	{
		rd = read(fd, b, BUFFER_SIZE);
		pos = 0;
		if(rd <= 0) return NULL;
	}

	int start = pos;
	while(pos < rd && b[pos] != '\n') pos++;

	int has_nl = (pos < rd && b[pos] == '\n');
	int len = pos - start + has_nl;

	char *part = malloc(len + 1);
	if(!part) return NULL;

	for (int i = 0; i < len; i++) part[i] = b[i + start];
	part[len] = '\0';

	pos += has_nl;

	if(has_nl) return part;

	char *rest = get_next_line(fd);
	if(!rest) return part;

	int part_len = len;
	int rest_len = 0;
	while(rest[rest_len]) rest_len++;

	char *full = malloc(part_len + rest_len + 1);

	int i;
	for (i = 0; i < part_len; i++)
		full[i] = part[i];

	for (int j = 0; j < rest_len; j++)
		full[i+j] = rest[j];

	full[part_len + rest_len] = '\0';

	free(part);
	free(rest);

	return full;
}

char	*ft_strdup(const char *s1)
{
	char	*str = (char *)malloc(sizeof(*s1) * (strlen(s1) + 1));
	if (!str) return (NULL);
	size_t i = 0;
	while (s1[i]) {
		str[i] = s1[i];
		i++;
	}
	str[i] = 0;
	return (str);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i = 0;
	while (i < n && (s1[i] || s2[i])) {
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int	ft_atoi(const char *str)
{
	int		i = 0;
	int		sign = 1;
	long	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)) i++;
	if (str[i] == '-' || str[i] == '+') {
		if (str[i] == '-') sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9') {
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	count_words(char const *s, char c)
{
	int	count = 0;
	int	i = 0;
	while (s[i]) {
		while (s[i] == c) i++;
		if (s[i]) count++;
		while (s[i] && s[i] != c) i++;
	}
	return (count);
}

static char	*word_dup(char const *s, char c)
{
	int i = 0;
	while (s[i] && s[i] != c) i++;
	char *word = (char *)malloc(sizeof(char) * (i + 1));
	if (!word) return (NULL);
	i = 0;
	while (s[i] && s[i] != c) {
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	if (!s) return (NULL);
	char **arr = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!arr) return (NULL);
	int i = 0;
	int j = 0;
	while (s[i]) {
		while (s[i] == c) i++;
		if (s[i]) {
			arr[j] = word_dup(&s[i], c);
			if (!arr[j]) return (NULL);
			j++;
		}
		while (s[i] && s[i] != c) i++;
	}
	arr[j] = NULL;
	return (arr);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start = 0;
	size_t	end = strlen(s1);

	while (s1[start] && strchr(set, s1[start])) start++;
	while (end > start && strchr(set, s1[end - 1])) end--;
	size_t len = end - start;
	char *str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str) return (NULL);
	strncpy(str, s1 + start, len);
	str[len] = '\0';
	return (str);
}

void	free_split(char **arr)
{
	int	i = 0;
	while (arr[i]) free(arr[i++]);
	free(arr);
}

int	parse_color(char *str)
{
	char	**rgb = ft_split(str, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2]) return (0);
	int r = ft_atoi(rgb[0]);
	int g = ft_atoi(rgb[1]);
	int b = ft_atoi(rgb[2]);
	free_split(rgb);
	return ((r << 16) | (g << 8) | b);
}

void	add_map_line(t_game *game, char *line)
{
	int leading_spaces = 0;
	while (line[leading_spaces] == ' ' || line[leading_spaces] == '\t') leading_spaces++;
	char *trimmed = ft_strtrim(line, " \t");
	int trimmed_len = strlen(trimmed);
	char *map_line = malloc(trimmed_len + leading_spaces + 1);
	memset(map_line, '1', leading_spaces);
	strcpy(map_line + leading_spaces, trimmed);
	map_line[trimmed_len + leading_spaces] = '\0';
	free(trimmed);
	int len = strlen(map_line);
	if (len > game->map.width) game->map.width = len;
	for (int j = 0; map_line[j]; j++) {
		if (map_line[j] == 'N' || map_line[j] == 'S' || map_line[j] == 'E' || map_line[j] == 'W') {
			game->player.x = j + 0.5;
			game->player.y = game->map.height + 0.5;
			if (map_line[j] == 'N') {
				game->player.dir_x = 0;
				game->player.dir_y = -1;
				game->player.plane_x = 0.66;
				game->player.plane_y = 0;
			} else if (map_line[j] == 'S') {
				game->player.dir_x = 0;
				game->player.dir_y = 1;
				game->player.plane_x = -0.66;
				game->player.plane_y = 0;
			} else if (map_line[j] == 'E') {
				game->player.dir_x = 1;
				game->player.dir_y = 0;
				game->player.plane_x = 0;
				game->player.plane_y = 0.66;
			} else if (map_line[j] == 'W') {
				game->player.dir_x = -1;
				game->player.dir_y = 0;
				game->player.plane_x = 0;
				game->player.plane_y = -0.66;
			}
			map_line[j] = '0';
		}
	}
	game->map.grid[game->map.height] = map_line;
	game->map.height++;
}

void	parse_line(t_game *game, char *line)
{
	if (!ft_strncmp(line, "NO ", 3))
		game->config.no_path = ft_strtrim(line + 3, " \t\n");
	else if (!ft_strncmp(line, "SO ", 3))
		game->config.so_path = ft_strtrim(line + 3, " \t\n");
	else if (!ft_strncmp(line, "WE ", 3))
		game->config.we_path = ft_strtrim(line + 3, " \t\n");
	else if (!ft_strncmp(line, "EA ", 3))
		game->config.ea_path = ft_strtrim(line + 3, " \t\n");
	else if (line[0] == 'F')
		game->config.floor_color = parse_color(line + 2);
	else if (line[0] == 'C')
		game->config.ceiling_color = parse_color(line + 2);
	else if (line[0] == '1' || line[0] == '0' || line[0] == ' ' || line[0] == '\t' || line[0] == 'N' || line[0] == 'S' || line[0] == 'E' || line[0] == 'W')
		add_map_line(game, line);
}

int	parse_file(char *file, t_game *game)
{
	int fd = open(file, O_RDONLY);
	if (fd < 0) {
		perror("Error");
		return (1);
	}
	char *line;
	while ((line = get_next_line(fd))) {
		parse_line(game, line);
		free(line);
	}
	close(fd);
	return (0);
}

void	render_walls(t_game *game)
{
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		// Calculate ray direction
		double camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
		game->ray.raydir_x = game->player.dir_x + game->player.plane_x * camera_x;
		game->ray.raydir_y = game->player.dir_y + game->player.plane_y * camera_x;

		// Map position
		game->ray.map_x = (int)game->player.x;
		game->ray.map_y = (int)game->player.y;

		// Delta distance
		game->ray.deltadist_x = fabs(1 / game->ray.raydir_x);
		game->ray.deltadist_y = fabs(1 / game->ray.raydir_y);

		// Step and side distance
		if (game->ray.raydir_x < 0) {
			game->ray.step_x = -1;
			game->ray.sidedist_x = (game->player.x - game->ray.map_x) * game->ray.deltadist_x;
		} else {
			game->ray.step_x = 1;
			game->ray.sidedist_x = (game->ray.map_x + 1.0 - game->player.x) * game->ray.deltadist_x;
		}
		if (game->ray.raydir_y < 0) {
			game->ray.step_y = -1;
			game->ray.sidedist_y = (game->player.y - game->ray.map_y) * game->ray.deltadist_y;
		} else {
			game->ray.step_y = 1;
			game->ray.sidedist_y = (game->ray.map_y + 1.0 - game->player.y) * game->ray.deltadist_y;
		}

		// DDA
		game->ray.hit = 0;
		while (!game->ray.hit) {
			if (game->ray.sidedist_x < game->ray.sidedist_y) {
				game->ray.sidedist_x += game->ray.deltadist_x;
				game->ray.map_x += game->ray.step_x;
				game->ray.side = 0;
			} else {
				game->ray.sidedist_y += game->ray.deltadist_y;
				game->ray.map_y += game->ray.step_y;
				game->ray.side = 1;
			}
			if (game->ray.map_x < 0 || game->ray.map_x >= game->map.width ||
				game->ray.map_y < 0 || game->ray.map_y >= game->map.height ||
				game->map.grid[game->ray.map_y][game->ray.map_x] == '1') {
				game->ray.hit = 1;
			}
		}

		// Perpendicular distance
		if (game->ray.side == 0)
			game->ray.perpwalldist = game->ray.sidedist_x - game->ray.deltadist_x;
		else
			game->ray.perpwalldist = game->ray.sidedist_y - game->ray.deltadist_y;

		// Texture
		int tex_num = 0;
		if (game->ray.side == 0) {
			if (game->ray.step_x > 0) tex_num = 2; // W
			else tex_num = 3; // E
		} else {
			if (game->ray.step_y > 0) tex_num = 1; // S
			else tex_num = 0; // N
		}

		// Wall X
		double wall_x;
		if (game->ray.side == 0) wall_x = game->player.y + game->ray.perpwalldist * game->ray.raydir_y;
		else wall_x = game->player.x + game->ray.perpwalldist * game->ray.raydir_x;
		wall_x -= floor(wall_x);
		int tex_x = (int)(wall_x * game->tex.textures[tex_num]->width);

		// Line height
		int line_height = (int)(SCREEN_HEIGHT / game->ray.perpwalldist);
		int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0) draw_start = 0;
		int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_end >= SCREEN_HEIGHT) draw_end = SCREEN_HEIGHT - 1;

		// Draw
		for (int y = draw_start; y <= draw_end; y++) {
			int tex_y = (int)((y - draw_start) / (double)line_height * game->tex.textures[tex_num]->height);
			uint32_t color = get_tex_pixel(game->tex.textures[tex_num], tex_x, tex_y);
			mlx_put_pixel(game->mlx.frame.img, x, y, color);
		}
	}
}

uint32_t	get_tex_pixel(mlx_texture_t *tex, int x, int y)
{
	if (x < 0 || x >= (int)tex->width || y < 0 || y >= (int)tex->height)
		return 0x000000FF; // Black
	int index = (y * tex->width + x) * 4;
	uint8_t r = tex->pixels[index];
	uint8_t g = tex->pixels[index + 1];
	uint8_t b = tex->pixels[index + 2];
	uint8_t a = tex->pixels[index + 3];
	return (r << 24) | (g << 16) | (b << 8) | a;
}

void render_minimap(t_game *game) {
    int mini_size = 200;
    int cell_size = mini_size / (game->map.width > game->map.height ? game->map.width : game->map.height);
    if (cell_size < 1) cell_size = 1;
    int offset_x = 10, offset_y = 10;

    // Draw map
    for (int i = 0; i < game->map.height; i++) {
        for (int j = 0; j < game->map.width; j++) {
            uint32_t color = (game->map.grid[i][j] == '1') ? 0x000000FF : 0xFFFFFF00; // Black for walls, white for empty
            for (int dy = 0; dy < cell_size; dy++) {
                for (int dx = 0; dx < cell_size; dx++) {
                    int px = offset_x + j * cell_size + dx;
                    int py = offset_y + i * cell_size + dy;
                    if (px < SCREEN_WIDTH && py < SCREEN_HEIGHT) mlx_put_pixel(game->mlx.frame.img, px, py, color);
                }
            }
        }
    }

    // Draw player
    int px = offset_x + (int)(game->player.x * cell_size);
    int py = offset_y + (int)(game->player.y * cell_size);
    for (int dy = -2; dy <= 2; dy++) {
        for (int dx = -2; dx <= 2; dx++) {
            int x = px + dx, y = py + dy;
            if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) mlx_put_pixel(game->mlx.frame.img, x, y, 0xFF0000FF); // Red
        }
    }

    // Draw rays (every 10th ray for better visibility)
    for (int x = 0; x < SCREEN_WIDTH; x += 10) {
        double cam_x = 2 * x / (double)SCREEN_WIDTH - 1;
        double ray_x = game->player.dir_x + game->player.plane_x * cam_x;
        double ray_y = game->player.dir_y + game->player.plane_y * cam_x;
        int map_x = (int)game->player.x;
        int map_y = (int)game->player.y;
        double delta_x = fabs(1 / ray_x);
        double delta_y = fabs(1 / ray_y);
        int step_x = (ray_x < 0) ? -1 : 1;
        int step_y = (ray_y < 0) ? -1 : 1;
        double side_x = (ray_x < 0) ? (game->player.x - map_x) * delta_x : (map_x + 1 - game->player.x) * delta_x;
        double side_y = (ray_y < 0) ? (game->player.y - map_y) * delta_y : (map_y + 1 - game->player.y) * delta_y;
        int hit = 0;
        while (!hit && map_x >= 0 && map_x < game->map.width && map_y >= 0 && map_y < game->map.height) {
            if (side_x < side_y) {
                side_x += delta_x;
                map_x += step_x;
            } else {
                side_y += delta_y;
                map_y += step_y;
            }
            if (game->map.grid[map_y][map_x] == '1') hit = 1;
        }
        // Draw line from player to hit, only within minimap area
        int end_x = offset_x + map_x * cell_size + cell_size / 2;
        int end_y = offset_y + map_y * cell_size + cell_size / 2;
        // Simple line drawing
        int dx = abs(end_x - px), dy = abs(end_y - py);
        int sx = px < end_x ? 1 : -1, sy = py < end_y ? 1 : -1;
        int err = dx - dy;
        while (1) {
            if (px >= offset_x && px < offset_x + mini_size && py >= offset_y && py < offset_y + mini_size) 
                mlx_put_pixel(game->mlx.frame.img, px, py, 0x00FF00FF); // Green, only in minimap
            if (px == end_x && py == end_y) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; px += sx; }
            if (e2 < dx) { err += dx; py += sy; }
        }
    }
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game *game = (t_game *)param;
	int changed = 0;

	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
	{
		mlx_close_window(game->mlx.mlx);
		return;
	}

	if (keydata.action != MLX_PRESS) return; // Only on press

	if (keydata.key == MLX_KEY_W)
	{
		int new_x = (int)(game->player.x + game->player.dir_x);
		int new_y = (int)(game->player.y + game->player.dir_y);
		if (new_x >= 0 && new_x < game->map.width && new_y >= 0 && new_y < game->map.height &&
			game->map.grid[new_y][new_x] != '1')
		{
			game->player.x += game->player.dir_x;
			game->player.y += game->player.dir_y;
			changed = 1;
		}
	}
	else if (keydata.key == MLX_KEY_S)
	{
		int new_x = (int)(game->player.x - game->player.dir_x);
		int new_y = (int)(game->player.y - game->player.dir_y);
		if (new_x >= 0 && new_x < game->map.width && new_y >= 0 && new_y < game->map.height &&
			game->map.grid[new_y][new_x] != '1')
		{
			game->player.x -= game->player.dir_x;
			game->player.y -= game->player.dir_y;
			changed = 1;
		}
	}
	else if (keydata.key == MLX_KEY_A)
	{
		int new_x = (int)(game->player.x - game->player.plane_x);
		int new_y = (int)(game->player.y - game->player.plane_y);
		if (new_x >= 0 && new_x < game->map.width && new_y >= 0 && new_y < game->map.height &&
			game->map.grid[new_y][new_x] != '1')
		{
			game->player.x -= game->player.plane_x;
			game->player.y -= game->player.plane_y;
			changed = 1;
		}
	}
	else if (keydata.key == MLX_KEY_D)
	{
		int new_x = (int)(game->player.x + game->player.plane_x);
		int new_y = (int)(game->player.y + game->player.plane_y);
		if (new_x >= 0 && new_x < game->map.width && new_y >= 0 && new_y < game->map.height &&
			game->map.grid[new_y][new_x] != '1')
		{
			game->player.x += game->player.plane_x;
			game->player.y += game->player.plane_y;
			changed = 1;
		}
	}
	else if (keydata.key == MLX_KEY_LEFT)
	{
		double angle = 3.141592653589793 / 10.0; // 45 degrees
		double cos_a = cos(-angle);
		double sin_a = sin(-angle);
		double old_dir_x = game->player.dir_x;
		game->player.dir_x = old_dir_x * cos_a - game->player.dir_y * sin_a;
		game->player.dir_y = old_dir_x * sin_a + game->player.dir_y * cos_a;
		double old_plane_x = game->player.plane_x;
		game->player.plane_x = old_plane_x * cos_a - game->player.plane_y * sin_a;
		game->player.plane_y = old_plane_x * sin_a + game->player.plane_y * cos_a;
		changed = 1;
	}
	else if (keydata.key == MLX_KEY_RIGHT)
	{
		double angle = 3.141592653589793 / 10.0; // 45 degrees
		double cos_a = cos(angle);
		double sin_a = sin(angle);
		double old_dir_x = game->player.dir_x;
		game->player.dir_x = old_dir_x * cos_a - game->player.dir_y * sin_a;
		game->player.dir_y = old_dir_x * sin_a + game->player.dir_y * cos_a;
		double old_plane_x = game->player.plane_x;
		game->player.plane_x = old_plane_x * cos_a - game->player.plane_y * sin_a;
		game->player.plane_y = old_plane_x * sin_a + game->player.plane_y * cos_a;
		changed = 1;
	}

	if (changed)
	{
		// Clear image
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				mlx_put_pixel(game->mlx.frame.img, x, y, 0x000000FF);
			}
		}

		// Draw ceiling and floor
		uint32_t ceiling_color = (game->config.ceiling_color << 8) | 0xFF;
		uint32_t floor_color = (game->config.floor_color << 8) | 0xFF;
		for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				mlx_put_pixel(game->mlx.frame.img, x, y, ceiling_color);
			}
		}
		for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {
				mlx_put_pixel(game->mlx.frame.img, x, y, floor_color);
			}
		}

		// Render walls
		render_walls(game);

		// Render minimap
		render_minimap(game);
	}
}

int main(int argc, char **argv)
{
	t_game game;

	if (argc != 2) {
		printf("Error\nUsage: ./cub3D map.cub\n");
		return 1;
	}

	// Initialize game structure
	game.map.grid = calloc(1024, sizeof(char *));
	game.map.height = 0;
	game.map.width = 0;
	game.config.no_path = NULL;
	game.config.so_path = NULL;
	game.config.we_path = NULL;
	game.config.ea_path = NULL;
	game.config.floor_color = 0;
	game.config.ceiling_color = 0;

	// Parse the map file
	if (parse_file(argv[1], &game)) return 1;

	// Load textures
	char cwd[1024];
	if (!getcwd(cwd, sizeof(cwd))) {
		printf("Error\nFailed to get current directory\n");
		return 1;
	}
	char full_no[2048]; sprintf(full_no, "%s/%s", cwd, game.config.no_path);
	char full_so[2048]; sprintf(full_so, "%s/%s", cwd, game.config.so_path);
	char full_we[2048]; sprintf(full_we, "%s/%s", cwd, game.config.we_path);
	char full_ea[2048]; sprintf(full_ea, "%s/%s", cwd, game.config.ea_path);
	printf("Loading: %s\n", full_no);
	game.tex.textures[0] = mlx_load_png(full_no); // N
	game.tex.textures[1] = mlx_load_png(full_so); // S
	game.tex.textures[2] = mlx_load_png(full_we); // W
	game.tex.textures[3] = mlx_load_png(full_ea); // E
	if (!game.tex.textures[0] || !game.tex.textures[1] || !game.tex.textures[2] || !game.tex.textures[3]) {
		printf("Error\nFailed to load textures\n");
		return 1;
	}
	// Initialize MLX42
	game.mlx.mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", true);
	if (!game.mlx.mlx) {
		printf("Error\nFailed to initialize MLX42\n");
		return 1;
	}

	// Set up key hook for escape
	mlx_key_hook(game.mlx.mlx, key_hook, &game);

	// Create image for rendering
	game.mlx.frame.img = mlx_new_image(game.mlx.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!game.mlx.frame.img) {
		printf("Error\nFailed to create image\n");
		mlx_terminate(game.mlx.mlx);
		return 1;
	}

	// Use parsed colors from the map file
	uint32_t ceiling_color = (game.config.ceiling_color << 8) | 0xFF; // Add alpha
	uint32_t floor_color = (game.config.floor_color << 8) | 0xFF;    // Add alpha

	// Draw ceiling (top half)
	for (int y = 0; y < SCREEN_HEIGHT / 2; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			mlx_put_pixel(game.mlx.frame.img, x, y, ceiling_color);
		}
	}

	// Draw floor (bottom half)
	for (int y = SCREEN_HEIGHT / 2; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			mlx_put_pixel(game.mlx.frame.img, x, y, floor_color);
		}
	}

	// Render walls using raycasting
	render_walls(&game);

	// Render minimap
	render_minimap(&game);

	// Put image to window
	mlx_image_to_window(game.mlx.mlx, game.mlx.frame.img, 0, 0);

	// Start the game loop
	mlx_loop(game.mlx.mlx);

	// Cleanup
	for (int i = 0; i < 4; i++) {
		if (game.tex.textures[i]) {
			mlx_delete_texture(game.tex.textures[i]);
		}
	}
	free_split(game.map.grid);
	free(game.config.no_path);
	free(game.config.so_path);
	free(game.config.we_path);
	free(game.config.ea_path);

	mlx_terminate(game.mlx.mlx);
	return 0;
}