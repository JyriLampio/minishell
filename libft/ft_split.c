/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 15:52:35 by alogvine          #+#    #+#             */
/*   Updated: 2023/11/28 14:48:03 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wordlen(const char *s, int c)
{
	int	i;

	i = 0;
	while (*s)
	{
		if (*s == c)
			break ;
		i++;
		s++;
	}
	return (i);
}

static int	count_words(char const *s, char c)
{
	int	i;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			s += ft_wordlen(s, c);
			i++;
		}
		else
			s++;
	}
	return (i);
}

static void	*freestr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;

	if (!s)
		return (0);
	i = count_words(s, c);
	str = malloc((i + 1) * sizeof(char *));
	if (!str)
		return (0);
	str[i] = 0;
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			str[i] = ft_substr(s, 0, ft_wordlen(s, c));
			if (!str[i])
				return (freestr(str));
			i++;
			s += ft_wordlen(s, c);
		}
		else
			s++;
	}
	return (str);
}
