/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:31:39 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/19 18:06:25 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freeargs(t_args *args)
{
	t_args	*curr;
	t_args	*temp;

	curr = args;
	while (curr)
	{
		temp = curr;
		free(curr->arg);
		curr = curr->next;
		free(temp);
	}
}

void	freecmds(t_cmds *cmds)
{
	t_cmds *curr;
	t_cmds *temp;

	freeargs(cmds->args);
	curr = cmds;
	while (curr)
	{
		temp = curr;
		free(curr->cmd);
		curr = curr->next;
		free(temp);
	}
}

void	freeenv(t_env *env)
{
	t_env *curr;
	t_env *temp;

	curr = env;
	while (curr)
	{
		temp = curr;
		free(curr->key);
		free(curr->value);
		curr = curr->next;
		free(temp);
	}
}

void	freeminishell(t_minishell *minishell)
{
	if (minishell)
	{
		if (minishell->env)
			freeenv(minishell->env);
		if (minishell->cmds)
			freecmds(minishell->cmds);
		free(minishell);
	}
}
