/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 20:55:26 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/17 21:14:29 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void free_split(char **split) 
{
    int i = 0;
    while (split[i]) {
        free(split[i]);
        i++;
    }
    free(split);
}
