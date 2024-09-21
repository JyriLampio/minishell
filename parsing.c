/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:29:48 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/21 21:36:18 by alogvine         ###   ########.fr       */
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

int	redir_syntax(char *s)
{
	int		i;

	i = 0;
	while (*s++)
	{
		if (*s == '\'' || *s == '"')
			s += qlen(s, *s) - 1;
		else if (*s == '<' || *s == '>')
		{
			i = check_arrow(s++);
			if (!*s)
				return (1);
			if (*s && *s == ' ' && i < 3)
			{
				while (*s && *s == ' ')
					s++;
				if (!*s || *s == '<' || *s == '>')
					return (1);
			}	
		}
		if (i > 2)
			return (i);
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

char	*parse_cmd(char *new, char *line)
{
	while (new && *line)
	{
		if (*line == ' ')
			line++;
		else if (*line == '\'' || *line == '"')
			line += qlen(line, *line);
		else if (*line == '<' || *line == '>')
			line += skip_arrow(line);
		else if (*line)
		{
			while (new && *line && *line != ' ' && *line != '<' && *line != '>')
				new = ft_cjoin(new, *line++);
		}
	}
	printf("NEW: %s\n", new);
	return (new);
}

char	*arrowjoin(char *new, char *line)
{
	if (new && *line && *(line + 1) && ((*line == '<' && *(line + 1) == '<' )
			|| (*line == '>' && *(line + 1) == '>')))
	{
		new = ft_cjoin(new, *line++);
		new = ft_cjoin(new, *line++);
		new = ft_cjoin(new, ' ');
	}
	else if (new && *line && (*line == '<' || *line == '>'))
	{
		new = ft_cjoin(new, *line++);
		new = ft_cjoin(new, ' ');
	}
	while (*line && *line == ' ')
		line++;
	while (new && *line && *line != ' ' && *line != '<' && *line != '>')
		new = ft_cjoin(new, *line++);
	return (new);
}

char	*redir_line(char *line)
{
	char	*new;

	new = ft_strdup("");
	while (new && *line)
	{
		if (*line == ' ')
			line++;
		else if (*line == '<' || *line == '>')
		{
			new = arrowjoin(new, line);
			line += skip_arrow(line);
			new = ft_cjoin(new, '|');
		}
		else if (new && *line)
		{
			while (new && *line && *line != ' ' && *line != '<' && *line != '>')
				new = ft_cjoin(new, *line++);
			new = ft_cjoin(new, '|');
		}
	}
	return (new);
}

char	**sort_redirs(char **redirs)
{
	int		i;
	int		n;
	int		t;
	char	*start;

	i = 0;
	n = 0;
	t = 0;
	while (redirs[i])
	{
		while (redirs[i] && (redirs[i][0] == '<' || redirs[i][0] == '>'))
			i++;
		if (redirs[i])
		{
			start = redirs[i];
			n = i;
			while (n > t)
			{
				redirs[n] = redirs[n - 1];
				n--;
			}
			redirs[t++] = start;
		}
		if (redirs[i])
			i++;
	}
	return (redirs);
}

char	*parse_redirs(char *line)
{
	char	*temp;
	char	**redirs;
	char	*new;

	new = ft_strdup("");
	temp = redir_line(line);
	redirs = pipesplit(temp, '|');
	if (!redirs || !new || !temp)
		return (0);
	redirs = sort_redirs(redirs);
	while (*redirs)
	{
		new = ft_strjoin(new, *redirs++);
		if (*redirs)
			new = ft_cjoin(new, ' ');
	}
	free(temp);
	if (redirs)
		freestr(redirs);
	printf("NEW: %s\n", new);
	return (new);
}
