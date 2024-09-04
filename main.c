/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:36:37 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/04 13:28:26 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_to_structs(t_minishell *minishell, char *line)
{
	int		i;
	int		n;
	char	**argline;
	char	**pipeline;
	char	*arg;
	char	*temp;

	i = 0;
	n = 1;
	if (*line)
	{
		i = 0;
		n = 0;
		pipeline = ft_split(line, '|');
		while (pipeline[n])
		{
			i = 0;
			minishell->pipe = 0;
			if (pipeline[n] && (ft_strchr(pipeline[n], '<') || ft_strchr(pipeline[n], '>')))
				parsing_cmds(minishell, pipeline[n]);
			else
			{
				arg = ft_strdup("");
				argline = ft_split(pipeline[n], ' ');
				minishell->cmd = ft_strdup(argline[i]);
				i++;
				if (argline[i] && !ft_strcmp("echo", minishell[n].cmd) && !ft_strcmp(argline[i], "-n"))
				{
					minishell->cmd = ft_strjoin(minishell[n].cmd, " -n");
					i++;
				}
				while (argline[i])
				{
					temp = arg;
					arg = ft_strjoin(arg, argline[i]);
					free(temp);
					i++;
					if (argline[i])
					{
						temp = arg;
						arg = ft_strjoin(arg, " ");
						free(temp);
					}
				}
				minishell->arg = arg;
				i = 0;
				while (argline[i])
					free(argline[i++]);
				free(argline);
			}
			n++;
		}
		i = 0;
		while (pipeline[i])
			free(pipeline[i++]);
		free(pipeline);
	}
	return (n);
}

void	do_command(t_minishell *minishell)
{
	check_builtins(minishell);
}

int	num_rows(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

t_env	*create_node(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = 0;
	return (new);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*current;
	t_env	*new;
	int		i;
	char	**str;

	i = 0;
	while (envp[i])
	{
		str = ft_split(envp[i], '=');
		if (!str)
			return (0);
		new = create_node(str[0], str[1]);
		free(str);
		if (!new)
			return (0);
		if (i == 0)
			env = new;
		else if (current)
			current->next = new;
		current = new;
		i++;
	}
	return (env);
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
	{
		ft_putstr_fd("Error: Malloc fail", 2);
		exit(1);
	}
	ft_bzero(minishell, sizeof(t_minishell));
	minishell->envp = envp;
	minishell->env = init_env(envp);
	minishell->cmds = 0;
	minishell->cmd = 0;
	minishell->arg = 0;
	minishell->pipe = 0;
	return (minishell);
}

int	check_syntax(char *line)
{
	int	i;

	i = 0;
	printf("%s\n", line);
	while (line[i])
	{
		if (line[i] == '|')
		{
			while (line[i] == ' ')
				i++;
			if (line[i] == '|')
			{
				ft_putstr_fd("Command not avalaible, please purchase premium for $420,69\n", 1);
				return (1);
			}
		}
		else if (line [i] == '&')
		{
			while (line[i] == ' ')
				i++;
			if (line[i] == '&')
			{
				ft_putstr_fd("Command not avalaible, please purchase premium for $420,69\n", 1);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_minishell	*minishell;
	int			cmds;

	(void)av;
	(void)envp;
	if (ac == 1)
	{
		minishell = init_minishell(envp);
		while (1)
		{
			line = readline("minishell> ");
			add_history(line);
			if (!line || !*line || check_syntax(line))
			{
				free(line);
				continue ;
			}
			cmds = add_to_structs(minishell, line);
			do_command(minishell);
		}
	}
	return (0);
}
