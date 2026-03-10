/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 19:00:18 by wilisson          #+#    #+#             */
/*   Updated: 2025/07/16 20:09:05 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*sa;
	unsigned char	*sb;

	i = 0;
	sa = (unsigned char *)s1;
	sb = (unsigned char *)s2;
	while (i < n)
	{
		if ((unsigned char)sa[i] != (unsigned char)sb[i])
			return ((unsigned char)sa[i] - (unsigned char)sb[i]);
		i++;
	}
	return (0);
}
