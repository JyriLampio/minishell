/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:43:23 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 08:49:57 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	wait_processes(int pid)
// int	wait_processes(int pid, int command_count)
{
	int	status;
	int	exit_code;

	exit_code = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = 128 + WTERMSIG(status);
	return (exit_code);
}

int	prepare_execution(t_minishell *minishell)
{
	minishell->pid = malloc(sizeof(int) * minishell->num_cmds);
	if (minishell->pid == NULL)
	{
		error_msg(minishell, "malloc failed", 0);
		return (-1);
	}
	return (0);
}

int	error_msg(t_minishell *minishell, char *file, int exit_code)
{
	if (minishell)
		minishell->exit_status = exit_code;
	ft_putstr_fd("minishell: ", 2);
	perror(file);
	return (exit_code);
}