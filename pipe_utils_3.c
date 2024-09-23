/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:53:10 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 22:44:46 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_single_cmd(t_minishell *minishell,
		t_cmds *current_cmd, char **envp, int i)
{
	if (i < minishell->num_cmds - 1)
	{
		if (pipe(minishell->pipefds) == -1)
		{
			perror("pipe ERROR");
			return (-1);
		}
	}
	minishell->pid[i] = fork();
	if (minishell->pid[i] == -1)
	{
		perror("fork ERROR");
		return (-1);
	}
	if (minishell->pid[i] == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_child_process(minishell, current_cmd, envp, i);
	}
	if (i < minishell->num_cmds - 1)
		close(minishell->pipefds[1]);
	return (0);
}

int	execute_cmds(t_minishell *minishell, char **envp)
{
	t_cmds	*current_cmd;
	int		i;

	i = 0;
	current_cmd = minishell->cmds;
	while (i < minishell->num_cmds)
	{
		if (execute_single_cmd(minishell, current_cmd, envp, i) == -1)
			return (-1);
		if (minishell->write_end != STDIN_FILENO)
			close(minishell->write_end);
		if (i < minishell->num_cmds - 1)
			minishell->write_end = minishell->pipefds[0];
		else if (minishell->num_cmds > 1)
			close(minishell->pipefds[0]);
		current_cmd = current_cmd->next;
		i++;
	}
	return (0);
}
