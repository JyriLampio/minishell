#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_PATH_LENGTH 1024

# include "minishell.h"



char *get_env_value(t_env *env_list, const char *key)
{
    t_env *tmp = env_list;
    while (tmp)
    {
        if (strcmp(tmp->key, key) == 0)
            return tmp->value;
        tmp = tmp->next;
    }
    return NULL; // Not found
}

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
            return strdup(full_path);
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

char **build_argv(char *cmd_path, t_args *args)
{
    int argc = 0;
    t_args *current_arg = args;
    while (current_arg) {
        argc++;
        current_arg = current_arg->next;
    }

    char **argv = malloc(sizeof(char *) * (argc + 2)); // cmd + args + NULL
    if (!argv)
        return NULL; // Handle memory allocation failure

    // The first element in argv is the command itself
    argv[0] = cmd_path;

    // Copy each argument from the linked list to argv
    current_arg = args;
    for (int i = 0; i < argc; i++) {
        argv[i + 1] = current_arg->arg; // Add arguments starting from argv[1]
        current_arg = current_arg->next;
    }

    // The last element in argv must be NULL
    argv[argc + 1] = NULL;
	// write(1, "Built argv\n", 11);
    return argv;
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

#include <fcntl.h>    // For open flags
#include <unistd.h>   // For dup2, close
#include <string.h>   // For strcmp
#include <stdio.h>    // For perror
#include <stdlib.h>   // For exit

t_redirs *create_redirection(t_redir_type type, const char *filename)
{
    t_redirs *new_redir = (t_redirs *)malloc(sizeof(t_redirs));
    if (!new_redir)
        return NULL;

    new_redir->type = type;
    new_redir->file = strdup(filename); // allocate and copy the filename
    new_redir->next = NULL;

    return new_redir;
}

// Add redirection to the first command when i == 0
void add_redirection_to_first_cmd(t_cmds *cmds, int i)
{
    if (i == 0 && cmds != NULL)
    {
		printf("Adding redirection to first command\n");
        t_redirs *new_redir = create_redirection(REDIR_IN, "input_file");
        t_redirs *new_redir2 = create_redirection(REDIR_OUT, "output_file1");
		t_redirs *new_redir3 = create_redirection(REDIR_OUT, "output_file2");
		t_redirs *new_redir4 = create_redirection(REDIR_APPEND, "output_file3");
        // t_redirection *new_redir2 = create_redirection(REDIR_APPEND, "output_file");
		// t_redirection *new_redir3 = create_redirection(REDIR_APPEND, "output_file");
		new_redir->next = new_redir2;
		new_redir2->next = new_redir3;
		new_redir3->next = new_redir4;
        if (!new_redir)
            return;
		printf("Created redirection: %p\n", new_redir);
        // Append the new redir to the list of redirs in the first command
        if (cmds->redirs == NULL)
        {
			ft_putstr_fd("Redirs is NULL\n", 2);
            cmds->redirs = new_redir;
        }
        else
        {
			ft_putstr_fd("Redirs is not NULL\n", 2);
            t_redirs *current = cmds->redirs;
			ft_putstr_fd("Current redirs\n", 2);
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = new_redir;
        }
    }
	
}

int handle_redirections(t_cmds *cmd)
{
    t_redirs *redir = cmd->redirs;
    int fd;

    while (redir)
    {
        switch (redir->type)
        {
            case REDIR_IN:
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
                break;

            case REDIR_OUT:
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
                break;

            case REDIR_APPEND:
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
                break;

            case REDIR_HEREDOC:
            {
                int pipefd[2];
                if (pipe(pipefd) == -1)
                {
                    perror("pipe");
                    return (-1);
                }
                // Read from stdin until delimiter is found
                char *line = NULL;
                size_t len = 0;
                ssize_t nread;

                while (1)
                {
                    write(STDOUT_FILENO, "> ", 2);
                    nread = getline(&line, &len, stdin);
                    if (nread == -1 || strcmp(line, redir->file) == 0)
                    {
                        free(line);
                        break;
                    }
                    write(pipefd[1], line, nread);
                }
                close(pipefd[1]);
                if (dup2(pipefd[0], STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    close(pipefd[0]);
                    return (-1);
                }
                close(pipefd[0]);
                break;
            }

            default:
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
                exit(1);

            // Set up pipe input
            if (in_fd != STDIN_FILENO) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }

            // Set up pipe output
            if (i < num_cmds - 1) {
                dup2(pipefds[1], STDOUT_FILENO);
                close(pipefds[1]);
            }

            // Close unused pipe end
            if (i < num_cmds - 1)
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
        if (i < num_cmds - 1)
            in_fd = pipefds[0];

        // Move to the next command in the list
        current_cmd = current_cmd->next;
    }

    // Parent process: wait for all child processes
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }
	ft_putstr_fd("FREEING PIPES\n", 1);
	freestr(envp);
    return 0;
}


//TOIMII JA REDIRECTAA MUT EI NORMINETTE
// int pipe_x(t_minishell *minishell)
// {
//     t_cmds *current_cmd = minishell->cmds;
//     int num_cmds = 0;
//     while (current_cmd != NULL) {
//         num_cmds++;
//         current_cmd = current_cmd->next;
//     }
// 	// KAKS FILE DESCRIPTORII YHDELLE PIPELLE
//     int pipefds[2]; 
//     pid_t pid;
//     char **envp = convert_env(minishell->env);
//     int in_fd = STDIN_FILENO;  // Initial input is from stdin
	

//     current_cmd = minishell->cmds;
//     for (int i = 0; i < num_cmds; i++) {
//         if (i < num_cmds - 1) {
//             if (pipe(pipefds) == -1) {
//                 perror("pipe ERROR");
//                 return 1;
//             }
//         }

//         pid = fork();
//         if (pid == -1) {
//             perror("fork ERROR");
//             return 1;
//         }

// 		if (pid == 0)  // Child process
// 		{
// 			// Redirect input from previous pipe if necessary
// 			if (in_fd != STDIN_FILENO)
// 			{
// 				dup2(in_fd, STDIN_FILENO);
// 				close(in_fd);
// 			}

// 			// Redirect output to next pipe if necessary
// 			if (i < num_cmds - 1)
// 			{
// 				dup2(pipefds[1], STDOUT_FILENO);
// 				close(pipefds[1]);
// 			}

// 			// Close unused file descriptors
// 			close(pipefds[0]);

// 			// Handle redirections
// 			if (handle_redirections(current_cmd) == -1)
// 				exit(1); // Exit if redirection fails

// 			// **Check if command is NULL**
// 			if (current_cmd->cmd == NULL)
// 			{
// 				// No command, just redirections
// 				exit(0);  // Exit after handling redirections
// 			}

// 			// Execute built-in or external command
// 			if (is_builtin(current_cmd->cmd))
// 			{
// 				execute_builtin(minishell, current_cmd);
// 				exit(0);
// 			}
// 			else
// 			{
// 				char *cmd_path = find_executable(current_cmd->cmd, minishell->env);
// 				char **argv = build_argv(cmd_path, current_cmd->args);

// 				execve(cmd_path, argv, envp);
// 				perror("execve ERROR");  // If execve fails
// 				exit(1);
// 			}
// 		}



//         // Parent process

//         // Close write end of the current pipe in the parent
//         if (i < num_cmds - 1) {
//             close(pipefds[1]);
//         }

//         // Close the previous read end, and prepare the new one
//         if (in_fd != STDIN_FILENO) {
//             close(in_fd);
//         }

//         // The input for the next command will be the current pipe's read end
//         in_fd = pipefds[0];

//         // Move to the next command in the list
//         current_cmd = current_cmd->next;
//     }

//     // Parent process: wait for all child processes
//     for (int i = 0; i < num_cmds; i++) {
//         wait(NULL);
//     }
// 	freestr(envp);

//     return 0;
// }
