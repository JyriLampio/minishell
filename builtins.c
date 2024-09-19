/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:44:43 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/19 18:00:57 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(t_minishell *minishell)
{
	ft_putstr_fd("exit\n", 1);
	freeminishell(minishell);
	exit (0);
}

void	cmd_echo(t_args *args)
{
	int		n;

	n = 0;
	while (args && args->arg[0] == '-' && args->arg[1] == 'n')
	{
		n = 1;
		args = args->next;
	}
	while (args)
	{
		ft_putstr_fd(args->arg, 1);
		if (args->next)
			ft_putstr_fd(" ", 1);
		args = args->next;
	}
	if (n == 0)
		ft_putstr_fd("\n", 1);
}

static void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

char	**arr_args(t_args *args)
{
	char	**str;
	int		i;
	t_args	*curr;

	i = 0;
	curr = args;
	while (curr)
	{
		curr = curr->next;
		i++;
	}
	str = malloc(sizeof(t_args) * (i + 1));
	if (!str)
		return (0);
	str[i] = 0;
	i = 0;
	while (args)
	{
		str[i++] = ft_strdup(args->arg);
		args = args->next;
	}
	return (str);
}

int	check_builtins(t_minishell *minishell)
{
	t_cmds	*cmds;

	cmds = minishell->cmds;
	while (cmds)
	{
		if (!ft_strcmp("echo", cmds->cmd))
			cmd_echo(cmds->args);
		else if (!ft_strcmp("cd", cmds->cmd))
			printf("BUILTIN: CD\n");
		else if (!ft_strcmp("pwd", cmds->cmd))
			printf("BUILTIN: PWD\n");
		else if (!ft_strcmp("export", cmds->cmd))
			printf("BUILTIN: export\n");
		else if (!ft_strcmp("unset", cmds->cmd))
			printf("BUILTIN: UNSET\n");
		else if (!ft_strcmp("env", cmds->cmd))
			print_env(minishell->env);
		else if (!ft_strcmp("exit", cmds->cmd))
			free_and_exit(minishell);
		else
			ft_putstr_fd("Checking execve...\n", 1);
		cmds = cmds->next;
	}
	return (0);
}
