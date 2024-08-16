/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:55:56 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/27 12:34:20 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	n_len(int n)
{
	int	i;

	i = 0;
	if (n < 1)
		i++;
	while (n)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static int	ft_abs(long int n)
{
	if (n < 0)
		return (n *= -1);
	return (n);
}

char	*ft_itoa(int n)
{
	char			*str;
	int				sign;
	int				l;
	unsigned int	i;

	sign = 0;
	if (n < 0)
		sign = 1;
	l = n_len(n);
	str = malloc(l + 1);
	if (!str)
		return (0);
	str[l] = 0;
	i = ft_abs(n);
	while (l--)
	{
		str[l] = 48 + i % 10;
		i /= 10;
	}
	if (sign)
		*str = 45;
	return (str);
}
