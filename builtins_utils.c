/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:55:26 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 08:09:32 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_environment_variable(t_env *env, char *key, char *value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			break ;
		}
		current = current->next;
	}
}

t_env	*ft_lstlast(t_env *lst)
{
	t_env	*current;

	current = lst;
	while (current)
	{
		if (current->next == 0)
			return (current);
		current = current->next;
	}
	return (current);
}

char	**arr_args(t_args *args)
{
	char	**str;
	int		i;
	t_args	*curr;

	i = 0;
	curr = args;
	while (curr)
	{
		curr = curr->next;
		i++;
	}
	str = malloc(sizeof(t_args) * (i + 1));
	if (!str)
		return (0);
	str[i] = 0;
	i = 0;
	while (args)
	{
		str[i++] = ft_strdup(args->arg);
		args = args->next;
	}
	return (str);
}

// A helper function to split the string on the first occurrence of '='
char	**split_on_first_equals(char *str)
{
	char	**result;
	char	*equal_sign;

	result = malloc(sizeof(char *) * 3);
	equal_sign = ft_strchr(str, '=');
	if (!equal_sign)
	{
		result[0] = ft_strdup(str);
		result[1] = NULL;
	}
	else
	{
		*equal_sign = '\0';
		result[0] = ft_strdup(str);
		result[1] = ft_strdup(equal_sign + 1);
	}
	result[2] = NULL;
	return (result);
}
