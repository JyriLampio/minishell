/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/04 10:27:33 by alogvine          #+#    #+#             */
/*   Updated: 2024/01/21 10:32:23 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		i;
	int		check;

	check = 0;
	va_start(args, s);
	i = 0;
	while (*s)
	{
		if (*s == '%')
		{
			if (!*s++)
				return (i);
			i += ft_check(s, args, &check);
		}
		else
			i += ft_putchar((int) *s, &check);
		if (check == -1)
			break ;
		s++;
	}
	va_end(args);
	if (check == -1)
		return (-1);
	return (i);
}
