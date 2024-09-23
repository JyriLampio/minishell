/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:34:04 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 14:34:06 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*qexpand(char *new, char *line, t_minishell *minishell)
{
	char	*temp;

	while (new && *line && *line != '"')
	{
		if (new && *line == '$' && (*(line + 1) == '"' || *(line + 1) == ' '
				|| !*(line + 1)))
			new = cjoin(new, *line++);
		else if (new && *line == '$' && *(line + 1) == '?')
		{
			temp = new;
			new = ft_strjoin(new, ft_itoa(minishell->exit_status));
			line += 2;
			free(temp);
		}
		else if (new && *line == '$' && *(line + 1) == '\'')
			new = cjoin(new, *line);
		else if (new && *line == '$')
		{
			new = expjoin(new, line, minishell->env);
			line += dlen(line);
		}
		else
			new = cjoin(new, *line++);
	}
	return (new);
}

char	*noexpand(char *new, char *line)
{
	line++;
	while (new && *line != '\'')
		new = cjoin(new, *line++);
	return (new);
}

char	*dexpand(char *new, char *line, t_minishell *minishell)
{
	if (new && (!*(line + 1) || *(line + 1) == ' '))
		new = cjoin(new, '$');
	else if (new && *(line + 1) && (*(line + 1) == '"' || *(line + 1) == '\''))
		line++;
	else if (new && *(line + 1) && *(line + 1) == '?')
	{
		new = ft_strjoin(new, ft_itoa(minishell->exit_status));
		line += 2;
	}
	else if (new && *line)
		new = expjoin(new, line, minishell->env);
	return (new);
}

char	*expand(char *line, t_minishell *minishell)
{
	char	*new;

	new = ft_strdup("");
	while (new && *line)
	{
		if (*line == '\'')
		{
			new = noexpand(new, line);
			line += qlen(line, *line);
		}
		else if (*line == '"')
		{
			new = qexpand(new, line + 1, minishell);
			line += qlen(line, *line);
		}
		else if (*line == '$')
		{
			new = dexpand(new, line, minishell);
			line += dlen(line);
		}
		else
			new = cjoin(new, *line++);
	}
	return (new);
}
