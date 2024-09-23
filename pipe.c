/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 08:17:33 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 08:23:38 by jlampio          ###   ########.fr       */
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
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2); 
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

int execute_single_command(t_minishell *minishell)
{
    int pid;
    char **envp = convert_env(minishell->env);
	ft_putstr_fd("Converted env\n", 1);

    pid = fork();
    if (pid == -1)
    {
        perror("fork ERROR");
        return 1;
    }

    if (pid == 0)  // Child process
    {
        char *cmd_path = find_executable(minishell->cmds->cmd, minishell->env);
        char **argv = build_argv(cmd_path, minishell->cmds->args);
        execve(cmd_path, argv, envp);
        perror("execve ERROR");
        exit(1);
    }
    waitpid(pid, NULL, 0);
    return 0;
}

int is_builtin(char *cmd) {
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") ||
            !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "exit"));
}

// UUUUUSUSU FUNKTIO REDIRECT ONGELMALLE
int has_output_redirection(t_redirs *redirs) {
    t_redirs *redir = redirs;
    while (redir != NULL) {
        if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND) {
            return 1;
        }
        redir = redir->next;
    }
    return 0;
}

int has_input_redirection(t_redirs *redirs) {
    t_redirs *redir = redirs;
    while (redir != NULL) {
        if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC) {
            return 1;
        }
        redir = redir->next;
    }
    return 0;
}

int handle_redirections(t_cmds *cmd)
{
    t_redirs *redir = cmd->redirs;
    int fd;

    while (redir)
    {
        if (redir->type == REDIR_IN)
        {
            fd = open(redir->file, O_RDONLY);
            if (fd < 0)
            {
                perror(redir->file);
                return (-1);
            }
            if (dup2(fd, STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd);
        }
        else if (redir->type == REDIR_OUT)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                return (-1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd);
        }
        else if (redir->type == REDIR_APPEND)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(redir->file);
                return (-1);
            }
            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                perror("dup2");
                close(fd);
                return (-1);
            }
            close(fd);
        }
        else if (redir->type == REDIR_HEREDOC)
        {
            int pipefd[2];
            if (pipe(pipefd) == -1)
            {
                perror("pipe");
                return (-1);
            }

			char *line;
			while (1)
			{
				line = readline("> ");
				if (!line || strcmp(line, redir->file) == 0)
				{
					free(line);
					break;
				}
				write(pipefd[1], line, ft_strlen(line));
				write(pipefd[1], "\n", 1);
				free(line);
			}
            close(pipefd[1]);
            if (dup2(pipefd[0], STDIN_FILENO) == -1)
            {
                perror("dup2");
                close(pipefd[0]);
                return (-1);
            }
            close(pipefd[0]);
        }
        else
        {
            fprintf(stderr, "Unknown redirection type\n");
            return (-1);
        }
        redir = redir->next;
    }
    return (0);
}

int pipe_x(t_minishell *minishell)
{
    t_cmds *current_cmd = minishell->cmds;
    int num_cmds = 0;

    while (current_cmd != NULL) {
        num_cmds++;
        current_cmd = current_cmd->next;
    }

    int pipefds[2]; 
    pid_t pid;
    char **envp = convert_env(minishell->env);
    int in_fd = STDIN_FILENO;  // Initial input is from stdin

    current_cmd = minishell->cmds;

    for (int i = 0; i < num_cmds; i++) {
        // Create a pipe for the next command, unless it's the last command
        if (i < num_cmds - 1) {
            if (pipe(pipefds) == -1) {
                perror("pipe ERROR");
                return 1;
            }
        }

        pid = fork();
        if (pid == -1) {
            perror("fork ERROR");
            return 1;
        }

		if (pid == 0) {  // Child process
			// Handle redirections first
			if (handle_redirections(current_cmd) == -1)
				exit(1); // Exit if redirection fails

			// Check if input is redirected
			int input_redirected = has_input_redirection(current_cmd->redirs);

			// Set up pipe input
			if (in_fd != STDIN_FILENO) {
				if (!input_redirected) {
					// Redirect input from the previous pipe's read end
					dup2(in_fd, STDIN_FILENO);
				}
				// Close in_fd in any case
				close(in_fd);
			}

			// Check if output is redirected to a file
			int output_redirected = has_output_redirection(current_cmd->redirs);

			if (i < num_cmds - 1) {
				if (!output_redirected) {
					// Redirect output to the next pipe's write end
					dup2(pipefds[1], STDOUT_FILENO);
				}
				// Close the pipe's write end
				close(pipefds[1]);
			}

			// Close unused pipe end
			if (num_cmds > 1)
				close(pipefds[0]);

			// Check if command is NULL
			if (current_cmd->cmd == NULL) {
				// No command, just redirections
				exit(0);  // Exit after handling redirections
			}

			// Execute built-in or external command
			if (is_builtin(current_cmd->cmd)) {
				execute_builtin(minishell, current_cmd);
				exit(0);
			} else {
				char *cmd_path = find_executable(current_cmd->cmd, minishell->env);
				char **argv = build_argv(cmd_path, current_cmd->args);

				execve(cmd_path, argv, envp);
				perror("execve ERROR");  // If execve fails
				exit(1);
			}
		}



			// Parent process

		// Close write end of the current pipe in the parent
		if (i < num_cmds - 1) {
			close(pipefds[1]);
		}

		// Close the previous read end, and prepare the new one
		if (in_fd != STDIN_FILENO) {
			close(in_fd);
		}

		// The input for the next command will be the current pipe's read end
		if (i < num_cmds - 1) {
			in_fd = pipefds[0];
		} else {
			// No more commands, close the read end
			if (num_cmds > 1)
			close(pipefds[0]);
		}

		// Move to the next command in the list
		current_cmd = current_cmd->next;

    }

    // Parent process: wait for all child processes
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }
	// ft_putstr_fd("FREEING PIPES\n", 1);
	freestr(envp);
    return 0;
}

