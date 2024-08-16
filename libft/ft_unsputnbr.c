/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsputnbr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 09:55:34 by alogvine          #+#    #+#             */
/*   Updated: 2024/01/21 09:56:09 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_unsputnbr(unsigned int i, int *check)
{
	int		count;
	char	c;

	count = 0;
	if (i / 10 != 0)
		count += ft_unsputnbr(i / 10, check);
	c = (i % 10) + '0';
	if (ft_putchar(c, check) == -1)
		return (-1);
	else
		return (count + 1);
}
