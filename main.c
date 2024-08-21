/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:36:37 by alogvine          #+#    #+#             */
/*   Updated: 2024/08/21 10:32:30 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_to_structs(t_minishell **minishell, char *line)
{
	int		i;
	int		n;
	char	**argline;
	char	**pipeline;
	char	*arg;
	char	*temp;

	i = 0;
	n = 1;
	while (line[i++])
		if (line[i] == '|')
			n++;
	*minishell = malloc(sizeof(t_minishell) * n);
	if (*line)
	{
		i = 0;
		n = 0;
		pipeline = ft_split(line, '|');
		while (pipeline[n])
		{
			i = 0;
			argline = ft_split(pipeline[n], ' ');
			(*minishell)[n].cmd = ft_strdup(argline[i]);
			i++;
			if (argline[i] && !ft_strcmp("echo", (*minishell)[n].cmd) && !ft_strcmp(argline[i], "-n"))
			{
				(*minishell)[n].cmd = ft_strjoin((*minishell)[n].cmd, " -n");
				i++;
			}
			arg = ft_strdup("");
			while (argline[i])
			{
				temp = arg;
				arg = ft_strjoin(arg, argline[i]);
				free(temp);
				if (argline[i + 1])
				{
					temp = arg;
					arg = ft_strjoin(arg, " ");
					free(temp);
				}
				i++;
			}
			(*minishell)[n].arg = arg;
			i = 0;
			while (argline[i])
				free(argline[i++]);
			free(argline);
			n++;
		}
		i = 0;
		while (pipeline[i])
			free(pipeline[i++]);
		free(pipeline);
	}
	return (n);
}

void	do_command(t_minishell *minishell, int cmds)
{
	int	i;

	i = 0;
	while (i < cmds)
	{
		check_builtins(minishell, i);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	(void)envp;
	char		*line;
	t_minishell	*minishell;
	int			cmds;

	if (ac == 1)
	{
		while (1)
		{
			line = readline("minishell> ");
			if (!line || !*line)
				continue ;
			cmds = add_to_structs(&minishell, line);
			do_command(minishell, cmds);
			free(minishell);
		}
	}
	return (0);
}
