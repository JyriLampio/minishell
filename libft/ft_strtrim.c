/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:14:00 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/27 12:38:17 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		n;
	char	*str;

	if (!s1 || !set)
		return (0);
	str = 0;
	i = 0;
	n = ft_strlen(s1) - 1;
	while (ft_strchr(set, s1[i]) && i <= n)
		i++;
	if (i > n)
		return (ft_strdup(s1 + n + 1));
	while (ft_strchr(set, s1[n]) && n >= 0)
		n--;
	str = malloc(n - i + 2);
	if (!str)
		return (0);
	ft_strlcpy(str, &s1[i], n - i + 2);
	return (str);
}
