/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 10:37:31 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 10:38:01 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void close_fd(int *fd)
{
    if (*fd != -1) {
        close(*fd);
        *fd = -1;
    }
}
// CLOSEE VIEL LISÄÄ JOS TARVII
void close_fds(t_minishell *minishell)
{
    close_fd(&minishell->pipefds[0]);
    close_fd(&minishell->pipefds[1]);
    if (minishell->write_end > 0)
        close(minishell->write_end);

}
