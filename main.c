/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:36:37 by alogvine          #+#    #+#             */
/*   Updated: 2024/08/16 13:04:40 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

void	add_to_structs(t_minishell *minishell, char *line)
{
	int		i;
	int		n;
	int		t;
	char	**argline;
	char	**pipeline;
	char	**arg;

	if (*line)
	{
		n = 0;
		pipeline = ft_split(line, '|');
		arg = malloc(sizeof(char **));
		arg[0][0] = 0;
		while (pipeline[n])
		{
			i = 0;
			t = 0;
			printf("PIPELINE[%d]: %s\n", n, pipeline[n]);
			argline = ft_split(pipeline[n], ' ');
			minishell[n].cmd = argline[i];
			i++;
			while (argline[i])
			{
				arg[n] = ft_strjoin(arg[n], argline[i]);
				if (argline[i + 1])
					arg[n] = ft_strjoin(arg[n], " ");
				i++;
			}
			minishell[n].arg = arg[n];
			n++;
		}
		i = 0;
		while (i < n)
		{
			printf("MINISHELL[%d].CMD: %s\n", i, minishell[i].cmd);
			printf("MINISHELL[%d].ARG: %s\n", i, minishell[i].arg);
			i++;
		}
	}
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	(void)envp;
	char		*line;
	t_minishell	*minishell;

	if (ac == 1)
	{
		minishell = malloc(sizeof(t_minishell));
		while (1)
		{
			line = readline("minishell> ");
			add_to_structs(minishell, line);
		}
	}
	exit(0);
}
