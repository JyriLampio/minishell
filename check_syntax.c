/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:32:48 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 14:32:49 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_quotes(char *line)
{
	char	c;

	while (*line)
	{
		if (*line == '\'' || *line == '"')
		{
			c = *line;
			line++;
			while (*line && *line != c)
				line++;
			if (!*line)
				return (1);
		}
		line++;
	}
	return (0);
}

int	check_double(char *line)
{
	while (*line)
	{
		if (*line && (*line == '\'' || *line == '"'))
			line += qlen(line, *line);
		else if (*line && (*line == '|' || *line == '&'))
		{
			if (*(line + 1) && *line == '|' && *(line + 1) == '|')
				return (1);
			else if (*line == '&' && *(line + 1) == '&')
				return (1);
			line++;
			while (*line && *line == ' ')
				line++;
			if (*line && (*line == '|' || *line == '&'))
				return (2);
		}
		else if (*line)
			line++;
	}
	return (0);
}

int	check_empty(char *line)
{
	char	*start;

	start = line;
	while (*line && *line == ' ')
		line++;
	if (!*line)
		return (1);
	if (*line == '|')
		return (printf("bobershell: syntax error\n"));
	while (*line && *(line + 1))
		line++;
	while (ft_strcmp(start, line) && *line != '|')
		line--;
	if (!*line)
		return (printf("bobershell: syntax error\n"));
	if (*line == '|')
	{
		while (*line && *line == ' ')
			line++;
		if (!*line)
			return (printf("bobershell: syntax error\n"));
	}
	return (0);
}

int	check_syntax(char *line)
{
	int	t;

	if (check_empty(line))
		return (1);
	else if (check_quotes(line))
		t = 2;
	else if (redir_syntax(line))
		t = 2;
	else if (check_arrow_file(line))
		t = 2;
	else
		t = check_double(line);
	if (t != 0)
	{
		if (t == 1)
			ft_putstr_fd("Command not available, \
please purchase premium for $420,69\n", 1);
		return (1);
	}
	return (0);
}
