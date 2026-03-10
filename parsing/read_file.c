/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 15:20:38 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/10 18:29:05 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	*read_file_into_buffer(const char *path, size_t *out_size)
{
	int		fd;
	char	*buf;
	char	*new_buf;
	size_t	total;
	size_t	cap;
	ssize_t	r;

	if (!path)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return (NULL);
	}
	cap = READ_CHUNK;
	buf = malloc(cap + 1);
	if (!buf)
	{
		perror("malloc");
		close(fd);
		return (NULL);
	}
	total = 0;
	while (1)
	{
		r = read(fd, buf + total, cap - total);
		if (r < 0)
		{
			perror("read");
			free(buf);
			close(fd);
			return (NULL);
		}
		if (r == 0)
			break ;
		total += (size_t)r;
		if (total >= cap)
		{
			cap *= 2;
			new_buf = malloc(cap + 1);
			if (!new_buf)
			{
				perror("malloc");
				free(buf);
				close(fd);
				return (NULL);
			}
			ft_memcpy(new_buf, buf, total);
			free(buf);
			buf = new_buf;
		}
	}
	buf[total] = '\0';
	if (out_size)
		*out_size = total;
	close(fd);
	return (buf);
}

char	**split_lines(const char *buf, size_t size, size_t *out_count)
{
	char	**lines;
	size_t	i;
	size_t	start;
	size_t	count;
	size_t	idx;

	if (!buf || size == 0)
	{
		if (out_count)
			*out_count = 0;
		return (NULL);
	}
	count = 0;
	i = 0;
	while (i < size)
	{
		if (buf[i] == '\n')
			++count;
		++i;
	}
	if (size > 0 && buf[size - 1] != '\n')
		++count;
	lines = malloc((count + 1) * sizeof(char *));
	if (!lines)
	{
		perror("malloc");
		if (out_count)
			*out_count = 0;
		return (NULL);
	}
	idx = 0;
	start = 0;
	i = 0;
	while (i <= size)
	{
		if (i == size || buf[i] == '\n')
		{
			size_t	len;
			char	*s;

			len = i - start;
			s = malloc(len + 1);
			if (!s)
			{
				perror("malloc");
				free_lines(lines);
				if (out_count)
					*out_count = 0;
				return (NULL);
			}
			if (len > 0)
				ft_memcpy(s, buf + start, len);
			s[len] = '\0';
			lines[idx++] = s;
			start = i + 1;
		}
		++i;
	}
	lines[idx] = NULL;
	if (out_count)
		*out_count = idx;
	return (lines);
}

void	free_lines(char **lines)
{
	size_t	i;

	if (!lines)
		return ;
	i = 0;
	while (lines[i])
	{
		free(lines[i]);
		++i;
	}
	free(lines);
}
