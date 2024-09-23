/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_structs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:09:42 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 14:14:04 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type	fetch_redir(char *redir)
{
	if (*redir && *(redir + 1) && *redir == '<' && *(redir + 1) == '<')
		return (REDIR_HEREDOC);
	if (*redir && *(redir + 1) && *redir == '>' && *(redir + 1) == '>')
		return (REDIR_APPEND);
	if (*redir && *redir == '<')
		return (REDIR_IN);
	if (*redir && *redir == '>')
		return (REDIR_OUT);
	return (0);
}

t_redirs	*make_redir(char *redir, char *file, t_minishell *minishell)
{
	t_redirs	*new;

	new = ft_calloc(1, sizeof(t_redirs));
	if (!new)
		return (0);
	new->type = fetch_redir(redir);
	new->file = expand(file, minishell);
	return (new);
}
