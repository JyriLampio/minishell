/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:49:19 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/22 15:46:58 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int i, size_t n)
{
	unsigned char	*str;
	size_t			t;

	t = 0;
	str = (unsigned char *) s;
	while (0 < n)
	{
		if (str[t] == (unsigned char) i)
			return (&str[t]);
		t++;
		n--;
	}
	if (*str == 0 && n == 0)
		return (NULL);
	return (0);
}
