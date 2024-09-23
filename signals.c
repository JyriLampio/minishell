/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 09:23:23 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 09:32:25 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	g_signum = 0;

void	handle_parent_signals(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_signum = sig;
	}
}