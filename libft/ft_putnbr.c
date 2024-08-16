/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 09:44:09 by alogvine          #+#    #+#             */
/*   Updated: 2024/01/21 10:01:41 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putnbr(int i, int *check)
{
	int		count;
	char	c;

	count = 0;
	if (i == -2147483648)
		return (ft_putstr("-2147483648", check));
	if (i < 0)
	{
		if (ft_putchar('-', check) == -1)
			return (-1);
		count = 1;
		i *= -1;
	}
	if (i / 10 != 0)
		count += ft_putnbr(i / 10, check);
	c = (i % 10) + '0';
	if (ft_putchar(c, check) == -1)
		return (-1);
	return (count + 1);
}
