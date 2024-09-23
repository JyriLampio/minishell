/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:36:52 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 19:51:45 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dlen(char *line)
{
	int	i;

	i = 0;
	if (*line && *line == '$' && *(line + 1) && *(line + 1) == '?')
		return (2);
	while (*line && *line != ' ' && *line != '\'' && *line != '"')
	{
		line++;
		i++;
	}
	return (i);
}

char	*cjoin(char *s1, char c)
{
	size_t	i;
	char	*str;

	if (!s1 || !c)
		return (0);
	i = 0;
	str = malloc(ft_strlen(s1) + 2);
	if (!str)
		return (0);
	while (s1[i] != 0)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = c;
	str[i + 1] = 0;
	free(s1);
	return (str);
}

char	*parse_key(char *new, char *key)
{
	char	*temp;

	if (new)
	{
		temp = new;
		new = ft_strdup("");
		if (new)
			return (new);
		new = ft_strjoin(new, key);
		free(temp);
	}
	if (!new)
		return ("");
	return (new);
}

char	*expjoin(char *new, char *line, t_env *env)
{
	char	*key;

	key = ft_strdup("");
	if (key && new && *line++ == '$')
	{
		while (key && *line && *line != ' ' && *line != '"' && *line != '\'')
			key = cjoin(key, *line++);
		while (new && key && env)
		{
			if (!ft_strcmp(key, env->key))
				new = parse_key(new, env->key);
			env = env->next;
		}
	}
	else if (key && new)
		new = cjoin(new, *line);
	free(key);
	return (new);
}
