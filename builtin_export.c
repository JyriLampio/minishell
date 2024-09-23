/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 15:08:24 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 16:05:24 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static void print_exported_env(t_env *env)
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
static void update_or_add_env(t_env *env, char **split)
{
    t_env *curr = env;

    while (curr)
    {
        if (ft_strcmp(curr->key, split[0]) == 0)
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
            return;
        }
        curr = curr->next;
    }
    t_env *new_env = malloc(sizeof(t_env));
    new_env->key = ft_strdup(split[0]);
    new_env->value = split[1] ? ft_strdup(split[1]) : NULL;
    new_env->next = NULL;
    t_env *last = ft_lstlast(env);
    last->next = new_env;
}


void builtin_export(t_env *env, t_args *args)
{
    char **split;

    if (!args)
    {
        print_exported_env(env);
        return;
    }
    while (args)
    {
        split = split_on_first_equals(args->arg);
        update_or_add_env(env, split);  // Use the helper function to update/add

        freestr(split);
        args = args->next;
    }
}

// void builtin_export(t_env *env, t_args *args)
// {
//     t_env *curr;
//     char **split;

//     if (!args)
//     {
//         print_exported_env(env);
//         return;
//     }

//     while (args)
//     {
//         split = split_on_first_equals(args->arg);
//         curr = env;

//         while (curr)
//         {
//             if (ft_strcmp(curr->key, split[0]) == 0)
//             {
//                 if (split[1])
//                 {
//                     free(curr->value);
//                     curr->value = ft_strdup(split[1]);
//                 }
//                 else
//                 {
//                     free(curr->value);
//                     curr->value = NULL;
//                 }
//                 break;
//             }
//             curr = curr->next;
//         }
//         if (!curr)
//         {
//             t_env *new_env = malloc(sizeof(t_env));
//             new_env->key = ft_strdup(split[0]);
//             new_env->value = split[1] ? ft_strdup(split[1]) : NULL;
//             new_env->next = NULL;
//             t_env *last = ft_lstlast(env);
//             last->next = new_env;
//         }
// 		freestr(split);
//         args = args->next;
//     }
// }
