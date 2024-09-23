/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 18:12:50 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 21:50:55 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_heredoc_input(int write_fd, const char *limiter)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        if (write(write_fd, line, strlen(line)) == -1)
        {
            perror("write");
            free(line);
            return (-1);
        }
        if (write(write_fd, "\n", 1) == -1)
        {
            perror("write");
            free(line);
            return (-1);
        }
        free(line);
    }
    return (0);
}

static int	handle_heredoc(t_redirs *redir)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    if (read_heredoc_input(pipefd[1], redir->file) == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    close(pipefd[1]);
    if (dup2(pipefd[0], STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(pipefd[0]);
        return (-1);
    }
    close(pipefd[0]);
    return (0);
}


int	handle_redir_heredoc(t_redirs *redir)
{
	int	status;

	status = handle_heredoc(redir);
	if (status == -1)
		return (-1);
	return (0);
}

int	handle_redir_append(t_redirs *redir)
{
	int	fd;

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
	return (0);
}
