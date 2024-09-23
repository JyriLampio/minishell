/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:44:43 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 16:03:52 by jlampio          ###   ########.fr       */
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
	while (args && args->arg[0] == '-' && args->arg[1] == 'n' && args->arg[2] == '\0')
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

int execute_builtin(t_minishell *minishell, t_cmds *current_cmd)
{
	t_cmds	*cmds;

	cmds = current_cmd;
	if (!ft_strcmp("echo", cmds->cmd))
		cmd_echo(cmds->args);
	else if (!ft_strcmp("cd", cmds->cmd))
		return(builtin_cd(arr_args(cmds->args), minishell->env));
	else if (!ft_strcmp("pwd", cmds->cmd))
		get_cwd();
	else if (!ft_strcmp("export", cmds->cmd))
		builtin_export(minishell->env, cmds->args);
	else if (!ft_strcmp("unset", cmds->cmd))
		builtin_unset(&minishell->env, cmds->args);
	else if (!ft_strcmp("env", cmds->cmd))
		print_env(minishell->env);
	else if (!ft_strcmp("exit", cmds->cmd))
		free_and_exit(minishell);
	return (0);
}

static int execute_single_builtin(t_minishell *minishell, t_cmds *current_cmd)
{
    int stdin_backup;
    int stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
    if (handle_redirections(current_cmd) == -1)
    {
        dup2(stdin_backup, STDIN_FILENO);
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdin_backup);
        close(stdout_backup);
        return (-1);
    }
    minishell->exit_status = execute_builtin(minishell, current_cmd);
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
	return (0);
}

int	check_builtins(t_minishell *minishell)
{
	t_cmds	*cmds;
	int		num_cmds;

	cmds = minishell->cmds;
	num_cmds = 0;
	while (cmds != NULL) {
        num_cmds++;
        cmds = cmds->next;
    }
	minishell->num_cmds = num_cmds;
	cmds = minishell->cmds;
	if (num_cmds == 1 && cmds->cmd && is_builtin(cmds->cmd))
		execute_single_builtin(minishell, cmds);
	else
	{
		pipe_x(minishell);
	}
	return (0);
}

