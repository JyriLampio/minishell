/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:29:48 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/19 17:39:04 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*parse_cmd(char *line)
{
	char	*new;

	new = ft_strdup("");
	while (*line)
	{
		if(*line == ' ')
			line += qlen(line, *line);
		else if (*line == '\'' || *line == '"')
			line += qlen(line, *line);
		else if (*line != '<' || *line != '>')
			while (*line != ' ' || *line != '<' || *line != '>')
				ft_cjoin(new, *line++);
	}
	return (new);
}

int	parse_redirs(char *line, t_minishell *minishell)
{
	char	*new;

	(void)minishell;
	new = parse_cmd(line);
//	new = parse_arg(line);
	printf("CMD IN REDIRLINE: %s\n", new);
	return (1);
}
