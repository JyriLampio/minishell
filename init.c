/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 13:34:55 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 13:35:32 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*make_node(char *key, char *value)
{
	t_env	*new;

	new = ft_calloc(1, sizeof(t_env));
	if (!new)
		return (0);
	new->key = ft_strdup(key);
	if (new->key && *value)
		new->value = ft_strdup(value);
	return (new);
}

void	set_shlvl(t_env *env)
{
	int		check;
	char	*temp;

	check = 0;
	while (env)
	{
		if (!ft_strcmp(env->key, "SHLVL"))
		{
			check = 1;
			break ;
		}
		env = env->next;
	}
	if (check)
	{
		temp = ft_itoa(ft_atoi(env->value) + 1);
		if (temp)
		{
			free(env->value);
			env->value = 0;
			env->value = temp;
		}
	}
	else
		env = make_node("SHLVL", "1");
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*current;
	t_env	*new;
	int		i;
	char	**str;

	i = 0;
	while (*envp && envp[i])
	{
		str = pipesplit(envp[i], '=');
		if (!str)
			return (0);
		new = make_node(str[0], str[1]);
		if (!new)
			return (0);
		freestr(str);
		if (i == 0)
			env = new;
		else if (current)
			current->next = new;
		current = new;
		i++;
	}
	set_shlvl(env);
	return (env);
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*minishell;

	minishell = ft_calloc(1, sizeof(t_minishell));
	if (!minishell)
		return (0);
	minishell->env = init_env(envp);
	return (minishell);
}
