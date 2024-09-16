/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:44:43 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/16 16:18:43 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(t_minishell *minishell)
{
	ft_putstr_fd("exit\n", 1);
	free(minishell);
	exit(0);
}

void	cmd_echo(char **args)
{
	int		i;
	char	*str;

	i = 0;
	str = ft_strdup("");
	while (args[i])
	{
		str = ft_strjoin(str, args[i++]);
		if (args[i])
			str = ft_strjoin(str, " ");
	}
	if (str[0] == '-' && str[1] == 'n')
		ft_putstr_fd(str + 3, 1);
	else
	{
		ft_putstr_fd(str, 1);
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
	if (args)
		printf("ARGS TO EXIST!!!\n");
	while (args)
	{
		str[i++] = ft_strdup(args->arg);
		printf("ARG ADDED:%s\n", args->arg);
		args = args->next;
	}
	return (str);
}

void	check_builtins(t_minishell *minishell)
{
	t_cmds	*cmds;

	cmds = minishell->cmds;
	while (cmds)
	{
		printf("CMD: %s\n", cmds->cmd);
		if (!ft_strcmp("echo", cmds->cmd))
			cmd_echo(arr_args(cmds->args));
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
}
