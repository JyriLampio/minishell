/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 16:03:36 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 16:23:11 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		ft_putstr_fd(current->key, STDOUT_FILENO);  // Print the key

		if (current->value != NULL)
		{
			ft_putstr_fd("=", STDOUT_FILENO);        // Print the '='
			ft_putstr_fd(current->value, STDOUT_FILENO);  // Print the value
		}
		
		ft_putstr_fd("\n", STDOUT_FILENO);  // Newline after each env variable
		current = current->next;
	}
}
