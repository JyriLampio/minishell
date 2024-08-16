/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:59:55 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/28 14:38:20 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int i, size_t n)
{
	unsigned char	c;
	unsigned char	*str;

	str = b;
	c = (unsigned char) i;
	while (n > 0)
	{
		*str++ = c;
		n--;
	}
	return (b);
}
