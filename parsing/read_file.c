/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 15:20:38 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/11 15:41:54 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static char	*grow_buffer(char *buf, size_t total, size_t *cap)
{
	char	*new_buf;

	*cap *= 2;
	new_buf = malloc(*cap + 1);
	if (!new_buf)
	{
		perror("malloc");
		free(buf);
		return (NULL);
	}
	ft_memcpy(new_buf, buf, total);
	free(buf);
	return (new_buf);
}

static char	*read_into_buf(int fd, char *buf, size_t *total, size_t *cap)
{
	ssize_t	r;

	while (1)
	{
		r = read(fd, buf + *total, *cap - *total);
		if (r < 0)
		{
			perror("read");
			free(buf);
			close(fd);
			return (NULL);
		}
		if (r == 0)
			break ;
		*total += (size_t)r;
		if (*total >= *cap)
		{
			buf = grow_buffer(buf, *total, cap);
			if (!buf)
				return (close(fd), NULL);
		}
	}
	return (buf);
}

char	*read_file_into_buffer(const char *path, size_t *out_size)
{
	int		fd;
	char	*buf;
	size_t	total;
	size_t	cap;

	if (!path)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (perror("open"), NULL);
	cap = READ_CHUNK;
	buf = malloc(cap + 1);
	if (!buf)
		return (perror("malloc"), close(fd), NULL);
	total = 0;
	buf = read_into_buf(fd, buf, &total, &cap);
	if (!buf)
		return (NULL);
	buf[total] = '\0';
	if (out_size)
		*out_size = total;
	close(fd);
	return (buf);
}
