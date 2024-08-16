/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 09:57:13 by alogvine          #+#    #+#             */
/*   Updated: 2024/06/26 13:09:29 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_check(const char *s, va_list args, int *check)
{
	int	count;

	if (*s == 'c')
		count = ft_putchar(va_arg(args, int), check);
	else if (*s == 's')
		count = ft_putstr(va_arg(args, char *), check);
	else if (*s == 'p')
		count = ft_pointer(va_arg(args, void *), check);
	else if (*s == 'u')
		count = ft_unsputnbr(va_arg(args, long int), check);
	else if (*s == 'i' || *s == 'd')
		count = ft_putnbr(va_arg(args, int), check);
	else if (*s == 'x')
		count = ft_hex(va_arg(args, unsigned int), 0, check);
	else if (*s == 'X')
		count = ft_hex(va_arg(args, unsigned int), 1, check);
	else if (*s == '%')
		count = ft_putchar('%', check);
	else
		count = ft_putchar((int) *s, check);
	if (*check == -1)
		count = -1;
	return (count);
}
