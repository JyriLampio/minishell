/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 13:41:45 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/13 14:01:18 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	n;
	size_t	t;

	if (dstsize == 0)
		return (ft_strlen(src));
	t = ft_strlen(dst) + ft_strlen(src);
	i = ft_strlen(dst);
	n = 0;
	if (dstsize < ft_strlen(dst))
		return (dstsize + ft_strlen(src));
	while (src[n] != 0 && i + 1 < dstsize)
	{
		dst[i] = src[n];
		i++;
		n++;
	}
	dst[i] = 0;
	return (t);
}
