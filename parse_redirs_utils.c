/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:51:19 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 14:51:21 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_arrow(char *s)
{
	int	i;

	i = 0;
	if (*s && *s == '<')
	{
		if (*(s + 1) && *(s + 1) == '>')
			return (3);
		while (*s && *s++ == '<')
			i++;
		return (i);
	}
	if (*s && *s == '>')
	{
		if (*(s + 1) && *(s + 1) == '<')
			return (3);
		while (*s && *s++ == '<')
			i++;
		return (i);
	}
	return (0);
}

int	check_arrow_file(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '<' || s[i] == '>')
		{
			while (s[i] && (s[i] == '<' || s[i] == '>'))
				i++;
			while (s[i] && s[i] == ' ')
				i++;
			if (!s[i] || (s[i] && (s[i] == '<' && s[i] == '>')))
			{
				if (s[i])
					printf("bobershell: syntax error\n");
				else if (!s[i])
					printf("bobershell: syntax error\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	redir_syntax(char *s)
{
	int	i;

	i = 0;
	while (*s)
	{
		if (*s == '\'' || *s == '"')
			s += qlen(s, *s) - 1;
		else if (*s && (*s == '<' || *s == '>'))
		{
			i = check_arrow(s++);
			if (!*s)
				return (printf("bobershell: syntax error\n"));
			if (*s && *s == ' ' && i < 3)
			{
				while (*s && *s == ' ')
					s++;
				if (!*s || *s == '<' || *s == '>')
					return (1);
			}
		}
		if (i > 2)
			return (printf("bobershell: syntax error\n"));
		s++;
	}
	return (0);
}

int	check_redirs(char *line)
{
	while (*line)
	{
		if (*line == '\'' || *line == '"')
			line += qlen(line, *line);
		else if (*line == '<' || *line == '>')
			return (1);
		else
			line++;
	}
	return (0);
}

int	skip_arrow(char *line)
{
	int	i;

	i = 1;
	if (*line && *(line + 1) && *(line + 1) == *line)
	{
		line += 2;
		i++;
	}
	else
		line++;
	if (*line)
	{
		while (*line && *line == ' ')
		{
			line++;
			i++;
		}
		while (*line && *line != ' ' && *line != '<' && *line != '>')
		{
			line++;
			i++;
		}
	}
	return (i);
}
