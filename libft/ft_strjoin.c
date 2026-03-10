/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 14:46:34 by wilisson          #+#    #+#             */
/*   Updated: 2025/07/22 14:34:19 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*null_cases(char const *s1, char const *s2)
{
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	return (NULL);
}

static void	cpy_strings(char *dst, char const *s1, char const *s2)
{
	while (*s1)
		*dst++ = *s1++;
	while (*s2)
		*dst++ = *s2++;
	*dst = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ptr;
	size_t	len_1;
	size_t	len_2;

	ptr = null_cases(s1, s2);
	if (ptr)
		return (ptr);
	len_1 = ft_strlen(s1);
	len_2 = ft_strlen(s2);
	ptr = malloc(len_1 + len_2 + 1);
	if (!ptr)
		return (NULL);
	cpy_strings(ptr, s1, s2);
	return (ptr);
}
