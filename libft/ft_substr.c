/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 12:37:50 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/27 12:38:30 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	n;

	i = 0;
	n = ft_strlen(s);
	if (start > n || !s)
		return ((char *)ft_calloc(1, 1));
	n = ft_strlen(s + start);
	if (n > len)
		n = len;
	str = malloc(n + 1);
	if (!str)
		return (0);
	while (i < len && s[start + i] != 0)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = 0;
	return (str);
}
