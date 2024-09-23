/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:36:37 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 22:58:19 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_exit;

int	add_to_structs(char *line, t_minishell *minishell)
{
	int		n;
	char	**pipeline;
	char	*redirline;

	n = 0;
	pipeline = pipesplit(line, '|');
	if (!pipeline || !*pipeline)
		return (1);
	while (pipeline[n])
	{
		if (check_redirs(pipeline[n]))
		{
			redirline = parse_redirs(pipeline[n++]);
			if (redirline)
				minishell->cmds = make_cmds(redirline, minishell, 1);
			free(redirline);
		}
		else
			minishell->cmds = make_cmds(pipeline[n++], minishell, 0);
	}
	freestr(pipeline);
	return (0);
}

int	do_command(t_minishell *minishell)
{
	if (check_builtins(minishell))
		return (1);
	return (0);
}

void	bobershell(t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		line = readline("bobershell> ");
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		signal(SIGINT, handle_parent_signals);
		add_history(line);
		if (!line || check_syntax(line))
		{
			free(line);
			continue ;
		}
		if (add_to_structs(line, minishell))
			return ;
		do_command(minishell);
		freecmds(minishell->cmds);
		minishell->cmds = 0;
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	*minishell;

	(void)ac;
	(void)av;
	signal(SIGINT, handle_parent_signals);
	signal(SIGQUIT, SIG_IGN);
	minishell = init_minishell(envp);
	bobershell(minishell);
	freeminishell(minishell);
	return (0);
}
