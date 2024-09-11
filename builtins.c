/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:44:43 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/11 17:16:07 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(t_minishell *minishell)
{
	ft_putstr_fd("exit\n", 1);
	free(minishell);
	exit(0);
}

void	cmd_echo(t_cmds *cmds)
{
	if (cmds->arg[0] == '-' && cmds->arg[1] == 'n')
		ft_putstr_fd(cmds->arg + 3, 1);
	else
	{
		ft_putstr_fd(cmds->arg, 1);
		ft_putstr_fd("\n", 1);
	}
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
	t_cmds	*cmds;

	cmds = minishell->cmds;
//	printf("CMD: %s\nARG: %s\n", cmds->cmd, cmds->arg);
	if (!ft_strcmp("echo", cmds->cmd))
		cmd_echo(minishell->cmds);
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
}
