/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:50:44 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/29 10:44:33 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int i)
{
	int		n;
	char	*str;

	n = 0;
	str = (char *) s;
	while (str[n] != (char) i)
	{
		if (str[n] == 0)
			return (0);
		n++;
	}
	return (&str[n]);
}
