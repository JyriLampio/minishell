/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:44:43 by alogvine          #+#    #+#             */
/*   Updated: 2024/08/21 13:02:49 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_and_exit(t_minishell *minishell)
{
	free(minishell);
	exit(0);
}

void	cmd_echo(t_minishell *minishell, int i, int line)
{
	if (line)
		printf("%s\n", minishell[i].arg);
	else
		printf("%s", minishell[i].arg);
}

void	check_builtins(t_minishell *minishell, int i)
{

	if (!ft_strcmp("echo", minishell[i].cmd))
		cmd_echo(minishell, i, 1);
	else if (!ft_strcmp("echo -n", minishell[i].cmd))
		cmd_echo(minishell, i, 0);
	else if (!ft_strcmp("cd", minishell[i].cmd))
		printf("BUILTIN: CD\n");
	else if (!ft_strcmp("pwd", minishell[i].cmd))
		printf("BUILTIN: PWD\n");
	else if (!ft_strcmp("export", minishell[i].cmd))
		printf("BUILTIN: EXPORT\n");
	else if (!ft_strcmp("unset", minishell[i].cmd))
		printf("BUILTIN: UNSET\n");
	else if (!ft_strcmp("env", minishell[i].cmd))
		printf("BUILTIN: ENV\n");
	else if (!ft_strcmp("exit", minishell[i].cmd))
		free_and_exit(minishell);
	else
		printf("CMD NOT FOUND!!!!!!!!!!!\n");
}
