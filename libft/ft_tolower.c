/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 14:05:04 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/13 14:05:09 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int i)
{
	if (i >= 65 && i <= 90)
		i += 32;
	return (i);
}
