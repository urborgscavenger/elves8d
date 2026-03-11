/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_lines.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 15:00:00 by wilisson          #+#    #+#             */
/*   Updated: 2026/03/11 15:57:34 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static size_t	count_lines(const char *buf, size_t size)
{
	size_t	count;
	size_t	i;

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
	return (count);
}

static char	*extract_line(const char *buf, size_t start, size_t end)
{
	size_t	len;
	char	*s;

	len = end - start;
	s = malloc(len + 1);
	if (!s)
		return (perror("malloc"), NULL);
	if (len > 0)
		ft_memcpy(s, buf + start, len);
	s[len] = '\0';
	return (s);
}

static int	fill_lines(char **lines, const char *buf, size_t size)
{
	size_t	idx;
	size_t	start;
	size_t	i;

	idx = 0;
	start = 0;
	i = 0;
	while (i <= size)
	{
		if (buf[i] == '\n' || (i == size && start < i))
		{
			lines[idx] = extract_line(buf, start, i);
			if (!lines[idx])
				return (free_lines(lines), -1);
			++idx;
			start = i + 1;
		}
		++i;
	}
	lines[idx] = NULL;
	return ((int)idx);
}

char	**split_lines(const char *buf, size_t size, size_t *out_count)
{
	char	**lines;
	size_t	count;
	int		idx;

	if (out_count)
		*out_count = 0;
	if (!buf || size == 0)
		return (NULL);
	count = count_lines(buf, size);
	lines = malloc((count + 1) * sizeof(char *));
	if (!lines)
		return (perror("malloc"), NULL);
	idx = fill_lines(lines, buf, size);
	if (idx < 0)
		return (NULL);
	if (out_count)
		*out_count = (size_t)idx;
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
