/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:17:33 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 12:02:51 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char *find_executable(char *cmd, t_env *env_list)
{
	int	i;
	char **dir;
    char full_path[1024];

    char *path = get_env_value(env_list, "PATH");
	if (!path)
	{
		printf("PATH not found\n");
		return cmd;
	}
	else if (cmd[0] == '/')
		return cmd;
	i = 0;
    if (!path)
    {
        printf("PATH not found\n");
        return NULL;
    }
	dir = ft_split(path, ':');
    while (dir[i])
    {
        full_path[0] = '\0';
        ft_strlcat(full_path, dir[i], sizeof(full_path));
        ft_strlcat(full_path, "/", sizeof(full_path));
        ft_strlcat(full_path, cmd, sizeof(full_path));
        if (access(full_path, X_OK) == 0)
        {
			free(path);
			freestr(dir);
            return ft_strdup(full_path);
        }
		i++;
    }
	free(path);
	freestr(dir);
    return NULL;
}


static char **convert_env(t_env *env_list)
{
    int count;
	int i;
    t_env *tmp = env_list;

	i = 0;
	count = 0;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;
    tmp = env_list;
	while (i < count)
    {
        size_t key_len = ft_strlen(tmp->key);
        // size_t value_len = ft_strlen(tmp->value);
		size_t value_len = tmp->value ? ft_strlen(tmp->value) : 0;
        envp[i] = malloc(key_len + value_len + 2);
        if (!envp[i])
            return NULL;
        envp[i][0] = '\0';
        ft_strlcat(envp[i], tmp->key, key_len + 1);
        ft_strlcat(envp[i], "=", key_len + 2);
        if (tmp->value)  // Only add the value if it exists
        {
            ft_strlcat(envp[i], tmp->value, key_len + value_len + 2);
        }

        tmp = tmp->next;
		i++;
    }
    envp[count] = NULL;
    return envp;
}

int is_builtin(char *cmd)
{
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") ||
            !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "exit"));
}

static void execute_child_process(t_minishell *minishell, t_cmds *current_cmd, char **envp, int index)
{
    if (handle_redirections(current_cmd) == -1)
        exit(1);
    if (dup_file_descriptors(minishell, current_cmd, index) == -1)
        exit(1);
    if (current_cmd->cmd == NULL)
        exit(0);
    if (is_builtin(current_cmd->cmd)) 
	{
        execute_builtin(minishell, current_cmd);
        exit(0);
    }
	else
	{
        char *cmd_path = find_executable(current_cmd->cmd, minishell->env);
        if (cmd_path == NULL) {
            ft_putstr_fd(current_cmd->cmd, 2);
			ft_putstr_fd(": command not found\n", 2); 
            exit(127); // 127 COMMAND NOT FOUNDEILLLELELELELEELELELELELELELE
        }
        char **argv = build_argv(cmd_path, current_cmd->args);
        execve(cmd_path, argv, envp);
        perror("execve ERROR");
        exit(1);
    }
}


static int execute_cmds(t_minishell *minishell, char **envp)
{
    t_cmds *current_cmd = minishell->cmds;
    int	i;
	
	i = 0;
	printf("num_cmds: %d\n", minishell->num_cmds);
    while (i < minishell->num_cmds) {
        // Create a pipe for the next command, unless it's the last command
        if (i < minishell->num_cmds - 1) {
            if (pipe(minishell->pipefds) == -1) {
                perror("pipe ERROR");
                return -1;
            }
        }
        minishell->pid[i] = fork();
        if (minishell->pid[i] == -1) {
            perror("fork ERROR");
            return -1;
        }
        if (minishell->pid[i] == 0) {  // Child process
            execute_child_process(minishell, current_cmd, envp, i);
        }
        // Parent process
        if (i < minishell->num_cmds - 1) {
            close(minishell->pipefds[1]);
        }
        if (minishell->write_end != STDIN_FILENO) {
            close(minishell->write_end);
        }
        if (i < minishell->num_cmds - 1) {
            minishell->write_end = minishell->pipefds[0];
        } else {
            if (minishell->num_cmds > 1)
                close(minishell->pipefds[0]);
        }
        current_cmd = current_cmd->next;
        i++;
    }
    return 0;
}

int pipe_x(t_minishell *minishell)
{
    char **envp;
    int i = 0;

    envp = convert_env(minishell->env);
    minishell->write_end = STDIN_FILENO;

    if (prepare_execution(minishell) == -1)
        return 1;

    if (execute_cmds(minishell, envp) == -1)
        return 1;

    // Wait for all child processes
    while (i < minishell->num_cmds)
	{
        minishell->exit_status = wait_processes(minishell->pid[i++]);
	}
	printf("exit_status: %d\n", minishell->exit_status);
    freestr(envp);
    return 0;
}

