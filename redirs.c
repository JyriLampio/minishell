/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:40:26 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 10:51:18 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static int handle_heredoc(t_redirs *redir)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }
    char *line;
    while (1) {
        line = readline("> ");
        if (!line || strcmp(line, redir->file) == 0) {
            free(line);
            break;
        }
        write(pipefd[1], line, strlen(line));
        write(pipefd[1], "\n", 1);
        free(line);
    }
    close(pipefd[1]);
    if (dup2(pipefd[0], STDIN_FILENO) == -1) {
        perror("dup2");
        close(pipefd[0]);
        return -1;
    }
    close(pipefd[0]);
    return 0;
}

// UUUUUSUSU FUNKTIO REDIRECT ONGELMALLE
int has_output_redirection(t_redirs *redirs)
{
    t_redirs *redir = redirs;
    while (redir != NULL) {
        if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND) {
            return 1;
        }
        redir = redir->next;
    }
    return (0);
}

int has_input_redirection(t_redirs *redirs)
{
    t_redirs *redir = redirs;
    while (redir != NULL) {
        if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC) {
            return 1;
        }
        redir = redir->next;
    }
    return (0);
}

int handle_redirections(t_cmds *cmd)
{
    t_redirs *redir = cmd->redirs;

    while (redir)
    {
        int fd = -1;
        switch (redir->type)
        {
            case REDIR_IN:
                fd = open(redir->file, O_RDONLY);
                if (fd < 0) {
                    perror(redir->file);
                    return -1;
                }
                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    close(fd);
                    return -1;
                }
                close(fd);
                break;
            case REDIR_OUT:
                fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror(redir->file);
                    return -1;
                }
                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    close(fd);
                    return -1;
                }
                close(fd);
                break;
            case REDIR_APPEND:
                fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0) {
                    perror(redir->file);
                    return -1;
                }
                if (dup2(fd, STDOUT_FILENO) == -1) {
                    perror("dup2");
                    close(fd);
                    return -1;
                }
                close(fd);
                break;
            case REDIR_HEREDOC:
                if (handle_heredoc(redir) == -1)
                    return -1;
                break;
            default:
                fprintf(stderr, "Unknown redirection type\n");
                return -1;
        }
        redir = redir->next;
    }
    return 0;
}
