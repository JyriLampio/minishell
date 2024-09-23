/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:08:24 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 18:30:51 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_exported_env(t_env *env)
{
	while (env)
	{
		if (env->value)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putstr_fd("=\"", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\"\n", STDOUT_FILENO);
		}
		else
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		env = env->next;
	}
}

// Function to update an existing environment variable
static void	update_env_value(t_env *curr, char **split)
{
	if (split[1])
	{
		free(curr->value);
		curr->value = ft_strdup(split[1]);
	}
	else
	{
		free(curr->value);
		curr->value = NULL;
	}
}

static void	add_new_env(t_env *env, char **split)
{
	t_env	*new_env;
	t_env	*last;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;

	new_env->key = ft_strdup(split[0]);

	if (split[1])
		new_env->value = ft_strdup(split[1]);
	else
		new_env->value = NULL;

	new_env->next = NULL;

	last = ft_lstlast(env);
	last->next = new_env;
}

static void	update_or_add_env(t_env *env, char **split)
{
	t_env	*curr;

	curr = env;
	while (curr)
	{
		if (ft_strcmp(curr->key, split[0]) == 0)
		{
			update_env_value(curr, split);
			return ;
		}
		curr = curr->next;
	}
	add_new_env(env, split);
}

void	builtin_export(t_env *env, t_args *args)
{
	char	**split;

	if (!args)
	{
		print_exported_env(env);
		return ;
	}
	while (args)
	{
		split = split_on_first_equals(args->arg);
		update_or_add_env(env, split);
		freestr(split);
		args = args->next;
	}
}
