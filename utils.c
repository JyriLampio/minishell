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

# include "minishell.h"

char *get_env_value(t_env *env_list, const char *key)
{
    t_env *tmp = env_list;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, key) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL;
}

char **build_argv(char *cmd_path, t_args *args)
{
    int argc = 0;
    t_args *current_arg = args;
    while (current_arg) {
        argc++;
        current_arg = current_arg->next;
    }

    char **argv = malloc(sizeof(char *) * (argc + 2)); // cmd + args + NULL
    if (!argv)
        return NULL; // Handle memory allocation failure

    // The first element in argv is the command itself
    argv[0] = cmd_path;

    // Copy each argument from the linked list to argv
    current_arg = args;
    for (int i = 0; i < argc; i++) {
        argv[i + 1] = current_arg->arg; // Add arguments starting from argv[1]
        current_arg = current_arg->next;
    }

    // The last element in argv must be NULL
    argv[argc + 1] = NULL;
	// write(1, "Built argv\n", 11);
    return argv;
}