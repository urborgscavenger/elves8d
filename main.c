/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbauer <mbauer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 12:00:00 by mbauer            #+#    #+#             */
/*   Updated: 2026/03/07 12:00:51 by mbauer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define BUFFER_SIZE 42

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
	game->map.grid[game->map.height] = ft_strdup(line);
	game->map.height++;
}

void	parse_line(t_game *game, char *line)
{
	if (!ft_strncmp(line, "NO ", 3))
		game->config.no_path = ft_strdup(line + 3);
	else if (!ft_strncmp(line, "SO ", 3))
		game->config.so_path = ft_strdup(line + 3);
	else if (!ft_strncmp(line, "WE ", 3))
		game->config.we_path = ft_strdup(line + 3);
	else if (!ft_strncmp(line, "EA ", 3))
		game->config.ea_path = ft_strdup(line + 3);
	else if (line[0] == 'F')
		game->config.floor_color = parse_color(line + 2);
	else if (line[0] == 'C')
		game->config.ceiling_color = parse_color(line + 2);
	else if (line[0] == '1' || line[0] == '0')
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

int main(int argc, char **argv)
{
	t_game game;

	if (argc != 2) {
		printf("Error\nUsage: ./cub3D map.cub\n");
		return 1;
	}

	// Initialize game structure
	game.map.grid = malloc(sizeof(char *) * 1024);
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

	// Initialize MLX42
	game.mlx.mlx = mlx_init(SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D", true);
	if (!game.mlx.mlx) {
		printf("Error\nFailed to initialize MLX42\n");
		return 1;
	}

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

	// Put image to window
	mlx_image_to_window(game.mlx.mlx, game.mlx.frame.img, 0, 0);

	// Start the game loop
	mlx_loop(game.mlx.mlx);

	// Cleanup
	mlx_terminate(game.mlx.mlx);
	return 0;
}