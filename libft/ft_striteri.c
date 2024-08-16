/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:44:39 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/22 14:59:38 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;
	unsigned int	n;

	if (!s)
		return ;
	i = 0;
	n = ft_strlen(s);
	while (i < n)
	{
		(*f)(i, s);
		i++;
		s++;
	}
}
