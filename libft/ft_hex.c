/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 09:56:23 by alogvine          #+#    #+#             */
/*   Updated: 2024/01/21 09:56:39 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_hex(unsigned long i, int upper, int *check)
{
	int	count;

	count = 0;
	if (i >= 16)
	{
		count += ft_hex((i / 16), upper, check);
		count += ft_hex((i % 16), upper, check);
		if (count == -1)
			return (-1);
	}
	else if ((upper == 0) && (i >= 10 && i <= 15))
		count += ft_putchar(i + 87, check);
	else if ((upper == 1) && (i >= 10 && i <= 15))
		count += ft_putchar(i + 55, check);
	else if (i < 10)
		count += ft_putnbr(i, check);
	return (count);
}
