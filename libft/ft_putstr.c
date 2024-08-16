/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 09:43:09 by alogvine          #+#    #+#             */
/*   Updated: 2024/06/26 13:10:15 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft/libft.h"

int	ft_putstr(char *s, int *check)
{
	int	len;

	if (!s)
		return (write(2, "(null)", 6));
	len = ft_strlen(s);
	if (write(2, s, len) == -1)
	{
		*check = -1;
		return (-1);
	}
	return (len);
}
