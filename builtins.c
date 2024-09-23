/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 11:44:43 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 08:09:57 by jlampio          ###   ########.fr       */
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

void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		ft_putstr_fd(current->key, STDOUT_FILENO);  // Print the key

		if (current->value != NULL)
		{
			ft_putstr_fd("=", STDOUT_FILENO);        // Print the '='
			ft_putstr_fd(current->value, STDOUT_FILENO);  // Print the value
		}
		
		ft_putstr_fd("\n", STDOUT_FILENO);  // Newline after each env variable
		current = current->next;
	}
}

void get_cwd(void)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        ft_putstr_fd(cwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
    }
    else
        perror("getcwd() error");
}

void print_exported_env(t_env *env)
{
    while (env)
    {
        if (env->value)
        {
            ft_putstr_fd("declare -x ", STDOUT_FILENO);
            ft_putstr_fd(env->key, STDOUT_FILENO);
            ft_putstr_fd("=\"", STDOUT_FILENO);
            ft_putstr_fd(env->value, STDOUT_FILENO);
            ft_putstr_fd("\"\n", STDOUT_FILENO);
        }
        else
        {
            ft_putstr_fd("declare -x ", STDOUT_FILENO);
            ft_putstr_fd(env->key, STDOUT_FILENO);
            ft_putstr_fd("\n", STDOUT_FILENO);
        }
        env = env->next;
    }
}

void builtin_export(t_env *env, t_args *args)
{
    t_env *curr;
    char **split;

    if (!args)
    {
        print_exported_env(env);
        return;
    }

    while (args)
    {
        split = split_on_first_equals(args->arg);
        curr = env;

        while (curr)
        {
            if (ft_strcmp(curr->key, split[0]) == 0)
            {
                if (split[1])
                {
                    free(curr->value);
                    curr->value = ft_strdup(split[1]);
                }
                else
                {
                    free(curr->value);
                    curr->value = NULL;
                }
                break;
            }
            curr = curr->next;
        }
        if (!curr)
        {
            t_env *new_env = malloc(sizeof(t_env));
            new_env->key = ft_strdup(split[0]);
            new_env->value = split[1] ? ft_strdup(split[1]) : NULL;
            new_env->next = NULL;
            t_env *last = ft_lstlast(env);
            last->next = new_env;
        }
		freestr(split);
        args = args->next;
    }
}

void builtin_unset(t_env **env, t_args *args)
{
    t_env *curr;
    t_env *prev;
    while (args) {
        curr = *env;
        prev = NULL;
        while (curr) {
            if (ft_strcmp(curr->key, args->arg) == 0) {
                if (prev == NULL)
                    *env = curr->next;
                else
                    prev->next = curr->next;
                free(curr->key);
                free(curr->value);
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        args = args->next;
    }
}

void execute_builtin(t_minishell *minishell, t_cmds *current_cmd)
{
	t_cmds	*cmds;

	cmds = current_cmd;
	if (!ft_strcmp("echo", cmds->cmd))
		cmd_echo(cmds->args);
	else if (!ft_strcmp("cd", cmds->cmd))
		builtin_cd(arr_args(cmds->args), minishell->env);
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
}

void execute_single_builtin(t_minishell *minishell, t_cmds *current_cmd)
{
    int stdin_backup;
    int stdout_backup;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);

    // Handle redirections
    if (handle_redirections(current_cmd) == -1)
    {
        // Restore original file descriptors before returning
        dup2(stdin_backup, STDIN_FILENO);
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdin_backup);
        close(stdout_backup);
        return;
    }

    // Execute the built-in command
    execute_builtin(minishell, current_cmd);

    // Restore original file descriptors
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
}


// int	check_builtins(t_minishell *minishell)
// {
// 	pipe_x(minishell);

// 	return (0);
// }

int	check_builtins(t_minishell *minishell)
{
	t_cmds	*cmds;
	cmds = minishell->cmds;
	if (cmds->next == NULL && cmds->cmd && is_builtin(cmds->cmd))  // Only one command
	{
		// printf("Single command builtin\n");
			// execute_builtin(minishell, cmds);
			execute_single_builtin(minishell, cmds);
		// else
		// 	execute_single_command(minishell);
	}
	else
	{
		printf("Piping\n");
		pipe_x(minishell);
		// execution(minishell);
	}
	return (0);
}

// void	check_builtins(t_minishell *minishell)
// {
// 	t_cmds	*cmds;
// 	write(1, "Checking builtins...\n", 21);
// 	cmds = minishell->cmds;

// 	if (!ft_strcmp("echo", cmds->cmd))
// 		builtin_echo(arr_args(cmds->args));
// 	else if (!ft_strcmp("cd", cmds->cmd))
// 		// printf("BUILTIN: CD\n");
// 		builtin_cd(arr_args(cmds->args));
// 	else if (!ft_strcmp("pwd", cmds->cmd))
// 		// printf("BUILTIN: PWD\n");
// 		get_cwd();
// 	else if (!ft_strcmp("export", cmds->cmd))
// 		// printf("BUILTIN: export\n");
// 		{
// 			printf("BUILTIN: EXPORT\n");
// 			builtin_export(minishell->env, cmds->args);
// 		}
// 	else if (!ft_strcmp("unset", cmds->cmd))
// 		// printf("BUILTIN: UNSET\n");
// 		builtin_unset(&minishell->env, cmds->args);
// 	else if (!ft_strcmp("env", cmds->cmd))
// 		print_env(minishell->env);
// 	else if (!ft_strcmp("exit", cmds->cmd))
// 		free_and_exit(minishell);
// 	else
// 	{
// 		ft_putstr_fd("Checking execve...\n", 1);
// 		if (minishell->cmds->next == NULL)  // Only one command
// 		{
// 			printf("Single command\n");
// 			execute_single_command(minishell);
// 			write(1, "Single commanDONEd\n", 16);
// 		}
// 		else  // More than one command, use piping
// 		{
// 			printf("Piping\n");
// 			pipe_x(minishell);
// 		}
// 	}
// }

