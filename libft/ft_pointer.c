/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 09:56:47 by alogvine          #+#    #+#             */
/*   Updated: 2024/01/21 09:57:08 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_pointer(void *ptr, int *check)
{
	int	count;

	count = 0;
	count += ft_putstr("0x", check);
	if (count == -1)
		return (-1);
	count += ft_hex((unsigned long)ptr, 0, check);
	return (count);
}
