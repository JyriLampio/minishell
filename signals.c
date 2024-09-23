/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:23:23 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 22:56:43 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit;

void	handle_parent_signals(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit = sig;
	}
}

void	sigint_handler_in_child(int sig)
{
	if (sig == SIGINT)
		printf("\n");
}

void	sigquit_handler_in_child(int sig)
{
	if (sig == SIGQUIT)
		printf("Quit (core dumped)\n");
}

void	sigint_handler_after_here_doc(int sig)
{
	if (sig == SIGINT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit = sig;
	}
}

void	sigint_handler_here_doc(int sig)
{
	if (sig == SIGINT)
		g_exit = SIGINT;
	close(STDIN_FILENO);
}
