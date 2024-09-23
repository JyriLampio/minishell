/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:02:33 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 16:18:07 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void builtin_unset(t_env **env, t_args *args)
{
    t_env *curr;
    t_env *prev;
    while (args) {
        curr = *env;
        prev = NULL;
        while (curr) {
            if (ft_strcmp(curr->key, args->arg) == 0) {
                if (prev == NULL)
                    *env = curr->next;
                else
                    prev->next = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        args = args->next;
    }
}
