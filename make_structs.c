/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_structs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:04:29 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 14:04:31 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args	*make_arg(char *arg, t_minishell *minishell)
{
	t_args	*new;

	new = ft_calloc(1, sizeof(t_args));
	if (!new)
		return (0);
	new->arg = expand(arg, minishell);
	return (new);
}

t_args	*make_args(t_args *targ, char *args, t_minishell *minishell)
{
	t_args	*new;
	t_args	*curr;

	curr = targ;
	new = make_arg(args, minishell);
	if (!new)
		return (0);
	if (targ == 0)
		targ = new;
	else if (curr)
	{
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	curr = new;
	return (targ);
}

t_redirs	*make_redirs(t_redirs *redirs, char *redir, char *file,
		t_minishell *minishell)
{
	t_redirs	*new;
	t_redirs	*curr;

	curr = redirs;
	new = make_redir(redir, file, minishell);
	if (!new)
		return (0);
	if (redirs == 0)
		redirs = new;
	else if (curr)
	{
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	curr = new;
	return (redirs);
}

t_cmds	*make_cmd(char *pipeline, t_minishell *minishell, int redir)
{
	t_cmds	*new;
	char	**args;
	int		i;

	i = 0;
	new = ft_calloc(1, sizeof(t_cmds));
	if (!new)
		return (0);
	args = pipesplit(pipeline, ' ');
	if (!args || !*args)
		return (0);
	if (args[i] && args[i][0] != '<' && args[i][0] != '>')
		new->cmd = expand(args[i++], minishell);
	while (args[i] && args[i][0] != '<' && args[i][0] != '>')
		new->args = make_args(new->args, args[i++], minishell);
	while ((redir == 1 && args[i]) && args[i + 1])
	{
		new->redirs = make_redirs(new->redirs, args[i], args[i + 1], minishell);
		i += 2;
	}
	new->next = 0;
	freestr(args);
	return (new);
}

t_cmds	*make_cmds(char *pipeline, t_minishell *minishell, int pipe)
{
	t_cmds	*current;
	t_cmds	*new;

	current = minishell->cmds;
	if (pipeline)
	{
		new = make_cmd(pipeline, minishell, pipe);
		if (!new)
			return (0);
		if (!minishell->cmds)
			minishell->cmds = new;
		else if (current)
		{
			while (current->next)
				current = current->next;
			current->next = new;
		}
		current = new;
	}
	return (minishell->cmds);
}
