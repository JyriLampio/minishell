/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:17:33 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 16:20:16 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static char *build_env_entry(t_env *env)
{
    size_t key_len; 
    size_t value_len;
	size_t total_len;
	
	key_len = ft_strlen(env->key);
	value_len = 0;
	total_len = key_len + 1 + value_len + 1;
	key_len = ft_strlen(env->key);
    if (env->value)
        value_len = ft_strlen(env->value);
    char *entry = malloc(key_len + value_len + 2);
    if (!entry)
        return NULL;
    entry[0] = '\0';
    ft_strlcat(entry, env->key, key_len + 1);
    ft_strlcat(entry, "=", key_len + 2);
	if (env->value)
	{
		ft_strlcat(entry, env->value, key_len + value_len + 2);
	}
	else
	{
        ft_strlcat(entry, " ", total_len + 1);
	}
    return entry;
}

static char **allocate_env_array(t_env *env_list, int *count)
{
    t_env *tmp;
    
	*count = 0;
	tmp = env_list;
    while (tmp)
    {
        (*count)++;
        tmp = tmp->next;
    }
    char **envp = malloc(sizeof(char *) * (*count + 1));
    if (!envp)
        return NULL;
    return envp;
}

static char **convert_env(t_env *env_list)
{
    int		count;
	int		i;
    char	**envp;
    t_env	*tmp; 
	
	tmp = env_list;
	envp = allocate_env_array(env_list, &count);
    if (!envp)
        return NULL;
    i = 0;
    while (i < count)
    {
        envp[i] = build_env_entry(tmp);
        if (!envp[i])
            return NULL;
        tmp = tmp->next;
        i++;
    }
    envp[count] = NULL;
    return envp;
}

void execute_child_process(t_minishell *minishell, t_cmds *current_cmd, char **envp, int index)
{
    if (handle_redirections(current_cmd) == -1)
        safe_exit(minishell, 1);
    if (dup_file_descriptors(minishell, current_cmd, index) == -1)
        safe_exit(minishell, 1);
    if (current_cmd->cmd == NULL)
        safe_exit(minishell, 1);
    if (is_builtin(current_cmd->cmd)) 
	{
        minishell->exit_status = execute_builtin(minishell, current_cmd);
        safe_exit(minishell, 1);
    }
	else
	{
        char *cmd_path = find_executable(current_cmd->cmd, minishell->env);
        if (cmd_path == NULL) 
		{
            ft_putstr_fd(current_cmd->cmd, 2);
			ft_putstr_fd(": command not found\n", 2);
			safe_exit(minishell, 127);
        }
        char **argv = build_argv(cmd_path, current_cmd->args);
        execve(cmd_path, argv, envp);
        perror("execve ERROR");
        safe_exit(minishell, 1);
    }
}

int pipe_x(t_minishell *minishell)
{
    char **envp;
    int i;
	
	i = 0;
    envp = convert_env(minishell->env);
    minishell->write_end = STDIN_FILENO;
    if (prepare_execution(minishell) == -1)
        return 1;
    if (execute_cmds(minishell, envp) == -1)
        return 1;
    while (i < minishell->num_cmds)
	{
        minishell->exit_status = wait_processes(minishell->pid[i++]);
	}
	if (minishell->pid)
	{
			free(minishell->pid);
			minishell->pid = 0;
	}
    freestr(envp);
    return 0;
}
