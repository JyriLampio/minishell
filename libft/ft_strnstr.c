/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:31:16 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/22 14:38:03 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = 0;
	if (ft_strlen(needle) == 0 || (!haystack && len == 0))
		return ((char *)haystack);
	while (haystack[n] != 0 && n < len)
	{
		i = 0;
		while (needle[i] != 0 && haystack[n + i] && n + i < len)
		{
			if (needle[i] != haystack[n + i])
				break ;
			i++;
		}
		if (needle[i] == 0)
			return ((char *)haystack + n);
		n++;
	}
	return (0);
}
