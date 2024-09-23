/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:17:02 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 08:19:48 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env_list, const char *key)
{
	t_env	*tmp;

	tmp = env_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	**build_argv(char *cmd_path, t_args *args)
{
	int		argc;
	t_args	*current_arg;
	char	**argv;
	int		i;

	i = 0;
	argc = 0;
	current_arg = args;
	while (current_arg)
	{
		argc++;
		current_arg = current_arg->next;
	}
	argv = malloc(sizeof(char *) * (argc + 2));
	if (!argv)
		return (NULL);
	argv[0] = cmd_path;
	current_arg = args;
	while (i < argc)
	{
		argv[i++ + 1] = current_arg->arg;
		current_arg = current_arg->next;
	}
	argv[argc + 1] = NULL;
	return (argv);
}
