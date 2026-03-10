/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wilisson <wilisson@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 18:35:42 by wilisson          #+#    #+#             */
/*   Updated: 2025/07/22 12:13:17 by wilisson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		len;
	int		sign;

	len = ft_numlen(n);
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	result[len] = '\0';
	sign = 1;
	if (n < 0)
		sign = -1;
	if (n == 0)
		result[0] = '0';
	while (n != 0)
	{
		len--;
		result[len] = '0' + sign * (n % 10);
		n /= 10;
	}
	if (sign == -1)
		result[0] = '-';
	return (result);
}
