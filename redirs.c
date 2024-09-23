/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:40:26 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 18:09:40 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc(t_redirs *redir)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, redir->file) == 0)
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, strlen(line));
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
	return (0);
}

int	has_output_redirection(t_redirs *redirs)
{
	t_redirs	*redir;

	redir = redirs;
	while (redir != NULL)
	{
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
			return (1);
		redir = redir->next;
	}
	return (0);
}

int	has_input_redirection(t_redirs *redirs)
{
	t_redirs	*redir;

	redir = redirs;
	while (redir != NULL)
	{
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
			return (1);
		redir = redir->next;
	}
	return (0);
}

int	handle_redir_in(t_redirs *redir)
{
	int	fd;

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
	return (0);
}

int	handle_redir_out(t_redirs *redir)
{
	int	fd;

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

int	handle_redir_heredoc(t_redirs *redir)
{
	int	status;

	status = handle_heredoc(redir);
	if (status == -1)
		return (-1);
	return (0);
}

int	handle_redirections(t_cmds *cmd)
{
	t_redirs	*redir;
	int			result;

	redir = cmd->redirs;
	while (redir)
	{
		result = 0;
		if (redir->type == REDIR_IN)
			result = handle_redir_in(redir);
		else if (redir->type == REDIR_OUT)
			result = handle_redir_out(redir);
		else if (redir->type == REDIR_APPEND)
			result = handle_redir_append(redir);
		else if (redir->type == REDIR_HEREDOC)
			result = handle_redir_heredoc(redir);
		if (result == -1)
			return (-1);
		redir = redir->next;
	}
	return (0);
}
