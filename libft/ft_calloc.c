/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:32:22 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/27 12:35:02 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>
#include <errno.h>

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*ptr;

	if (count * size > SIZE_MAX)
		return (0);
	ptr = malloc(count * size);
	if (!ptr)
	{
		errno = ENOMEM;
		return (0);
	}
	ft_bzero(ptr, count * size);
	return ((void *) ptr);
}
