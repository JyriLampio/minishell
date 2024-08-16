/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:51:45 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/13 17:22:54 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strrchr(const char *s, int i)
{
	char	*ptr;

	ptr = 0;
	while (*s)
	{
		if (*s == (char) i)
			ptr = (char *) s;
		s++;
	}
	if (*s == 0 && (char) i == 0)
		return ((char *) s);
	return (ptr);
}
