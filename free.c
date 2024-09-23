/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 14:31:39 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/22 13:48:09 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freeargs(t_args *args)
{
	t_args	*temp;

	while (args)
	{
		temp = args;
		free(args->arg);
		args = args->next;
		free(temp);
	}
}

void	freeredirs(t_redirs *redirs)
{
	t_redirs	*temp;

	while (redirs)
	{
		temp = redirs;
		free(redirs->file);
		redirs = redirs->next;
		free(temp);
	}
}

void	freecmds(t_cmds *cmds)
{
	t_cmds	*temp;

	if (cmds->args)
		freeargs(cmds->args);
	while (cmds)
	{
		temp = cmds;
		free(cmds->cmd);
		if (cmds->redirs)
			freeredirs(cmds->redirs);
		cmds = cmds->next;
		free(temp);
	}
}

void	freeenv(t_env *env)
{
	t_env	*temp;

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
		free(minishell->pid);
		minishell->pid = 0;
		free(minishell);
	}
	rl_clear_history();
}
