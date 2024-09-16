/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipesplit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:15:50 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/16 16:27:33 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	qlen(char *line, char c)
{
	int	i;

	i = 1;
	while (line[i] != c)
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
		if (*line == c)
			break ;
		if (*line == '\'' || *line == '"')
		{
			i += qlen(line, *line);
			line += qlen(line, *line);
		}
		else
		{
			i++;
			line++;
		}
	}
	return (i);
}

int	count_pipes(char *line, char c)
{
	int	i;

	i = 0;
	while (*line)
	{
		if (*line == '\'' || *line == '"')
			line += qlen(line, *line);
		else if (*line && *line != c)
		{
			i++;
			line += pipelen(line, c);
		}
		else
			line++;
	}
	return (i);
}

int	freestr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
	return (0);
}

char	**pipesplit(char *line, char c)
{
	char	**str;
	int		i;

	if (!line)
		return (0);
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
			while (*line == ' ')
				line++;
			str[i] = ft_substr(line, 0, pipelen(line, c));
			if (!str[i])
			{
				freestr(str);
				return (0);
			}
			i++;
			line += pipelen(line, c);
		}
		else
			line++;
	}
	return (str);
}
