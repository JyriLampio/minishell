/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipesplit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:15:50 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/22 11:52:19 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	qlen(char *line, char c)
{
	int	i;

	i = 1;
	while (line[i] && line[i] != c)
		i++;
	i++;
	return (i);
}

int	pipelen(char *line, char c)
{
	int	i;

	i = 0;
	while (*line)
	{
		if (*line && (*line == '\'' || *line == '"'))
		{
			i += qlen(line, *line);
			line += qlen(line, *line);
			continue ;
		}
		if (*line == c)
			break ;
		i++;
		line++;
	}
	return (i);
}

int	count_pipes(char *line, char c)
{
	int	i;

	i = 0;
	while (*line)
	{
		if (*line != c)
		{
			while (*line == '\'' || *line == '"')
				line += qlen(line, *line);
			line += pipelen(line, c);
			i++;
		}
		else
			line++;
	}
	return (i);
}

char	**freestr(char **str)
{
	char	**start;

	start = str;
	while (*str)
		free(*str++);
	if (*start)
		free(start);
	return (0);
}

char	**pipesplit(char *line, char c)
{
	char	**str;
	int		i;

	i = count_pipes(line, c);
	str = malloc(sizeof(str) * (i + 1));
	if (!str)
		return (0);
	str[i] = 0;
	i = 0;
	while (*line)
	{
		if (*line != c)
		{
			str[i] = ft_substr(line, 0, pipelen(line, c));
			if (!str[i++])
				return (freestr(str));
			line += pipelen(line, c);
		}
		else
			line++;
	}
	return (str);
}
