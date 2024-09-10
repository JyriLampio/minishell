/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:44:43 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/04 14:24:06 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(t_minishell *minishell)
{
	ft_putstr_fd("exit\n", 1);
	free(minishell);
	exit(0);
}

void	cmd_echo(t_minishell *minishell, int line)
{
	if (line)
	{
		ft_putstr_fd(minishell->arg, 1);
		ft_putstr_fd("\n", 1);
	}
	else
		ft_putstr_fd(minishell->arg, 1);
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

void	check_builtins(t_minishell *minishell)
{
	if (!ft_strcmp("echo", minishell->cmd))
		cmd_echo(minishell, 1);
	else if (!ft_strcmp("echo -n", minishell->cmd))
		cmd_echo(minishell, 0);
	else if (!ft_strcmp("cd", minishell->cmd))
		printf("BUILTIN: CD\n");
	else if (!ft_strcmp("pwd", minishell->cmd))
		printf("BUILTIN: PWD\n");
	else if (!ft_strcmp("export", minishell->cmd))
		printf("BUILTIN: export\n");
	else if (!ft_strcmp("unset", minishell->cmd))
		printf("BUILTIN: UNSET\n");
	else if (!ft_strcmp("env", minishell->cmd))
		print_env(minishell->env);
	else if (!ft_strcmp("exit", minishell->cmd))
		free_and_exit(minishell);
	else
		ft_putstr_fd("Checking execve...\n", 1);
}
