/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:31:39 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/21 17:50:41 by alogvine         ###   ########.fr       */
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

void	freeredirs(t_redirs *redirs)
{
	t_redirs	*curr;
	t_redirs	*temp;

	curr = redirs;
	while (curr)
	{
		temp = curr;
		free(redirs->file);
		curr = curr->next;
		free(temp);
	}
}

void	freecmds(t_cmds *cmds)
{
	t_cmds	*curr;
	t_cmds	*temp;

	freeargs(cmds->args);
	curr = cmds;
	while (curr)
	{
		temp = curr;
		free(curr->cmd);
		if (curr->redirs)
			freeredirs(curr->redirs);
		curr = curr->next;
		free(temp);
	}
}

void	freeenv(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (env)
	{
		temp = env;
		free(env->key);
		free(env->value);
		env = env->next;
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
//	rl_clear_history();
}
