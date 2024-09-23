/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   duplicators.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:33:02 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 10:37:03 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	dup_stdin(t_minishell *minishell, t_cmds *cmd)
{
	int	input_redirected;

	input_redirected = has_input_redirection(cmd->redirs);
	if (minishell->write_end != STDIN_FILENO)
	{
		if (!input_redirected)
		{
			if (dup2(minishell->write_end, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(minishell->write_end);
				return (-1);
			}
		}
		close(minishell->write_end);
	}
	return (0);
}

int	dup_stdout(t_minishell *minishell, t_cmds *cmd, int index)
{
	int	output_redirected;

	output_redirected = has_output_redirection(cmd->redirs);
	if (index < minishell->num_cmds - 1)
	{
		if (!output_redirected)
		{
			if (dup2(minishell->pipefds[1], STDOUT_FILENO) == -1)
			{
				perror("dup2");
				close(minishell->pipefds[1]);
				return (-1);
			}
		}
		close(minishell->pipefds[1]);
	}
	return (0);
}

int	dup_file_descriptors(t_minishell *minishell, t_cmds *cmd, int index)
{
	if (dup_stdin(minishell, cmd) == -1)
	{
		minishell->exit_status = errno;
		perror("dup_stdin");
		return (-1);
	}
	if (dup_stdout(minishell, cmd, index) == -1)
	{
		minishell->exit_status = errno;
		perror("dup_stdout");
		return (-1);
	}
	return (0);
}
