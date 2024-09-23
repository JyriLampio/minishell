/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:29:48 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/22 21:31:47 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*arrowjoin(char *new, char *line)
{
	if (new && *line && *(line + 1) && ((*line == '<' && *(line + 1) == '<')
			|| (*line == '>' && *(line + 1) == '>')))
	{
		new = cjoin(new, *line++);
		new = cjoin(new, *line++);
		new = cjoin(new, ' ');
	}
	else if (new && *line && (*line == '<' || *line == '>'))
	{
		new = cjoin(new, *line++);
		new = cjoin(new, ' ');
	}
	while (*line && *line == ' ')
		line++;
	while (new && *line && *line != ' ' && *line != '<' && *line != '>')
		new = cjoin(new, *line++);
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
			new = cjoin(new, '|');
		}
		else if (new && *line)
		{
			while (new && *line && *line != ' ' && *line != '<' && *line != '>')
				new = cjoin(new, *line++);
			new = cjoin(new, '|');
		}
	}
	return (new);
}

void	sort_redirs(char **redirs)
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
			n = i++;
			while (t < n)
			{
				redirs[n] = redirs[n - 1];
				n--;
			}
			redirs[t++] = start;
		}
	}
}

char	*parse_redirs(char *line)
{
	char	*temp;
	char	**redirs;
	char	*new;
	int		i;

	i = 0;
	new = ft_strdup("");
	temp = redir_line(line);
	redirs = pipesplit(temp, '|');
	if (temp)
		free(temp);
	if (!redirs || !new)
		return (0);
	sort_redirs(redirs);
	while (new && redirs[i])
	{
		temp = new;
		new = ft_strjoin(new, redirs[i++]);
		free(temp);
		if (new && *(redirs + i))
			new = cjoin(new, ' ');
	}
	if (redirs)
		freestr(redirs);
	return (new);
}
