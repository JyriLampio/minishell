/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:29:48 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/04 15:26:09 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_cmd(t_minishell *minishell, char *argline)
{
	int	i;

	i = 0;
	while (argline[i])
	{
		if (argline[i] == '<' || argline[i] == '>')
		{
			minishell->cmd = ft_strdup(argline);
			printf("%s\n", minishell->cmd);
		}
		i++;
	}
}

void	parsing_redirs(t_minishell *minishell, char *argline)
{
	int	i;

	i = 0;
	minishell->cmd = 0;
//	find_cmd(minishell, argline);
	while (argline[i++])
	{
//		printf("I DO BE HERE\nline: %s\n", argline);
		if (argline[i] == '<' && argline[i + 1] && argline[i + 1] == '<')
		{
			printf("HeH?\n");
			minishell->cmd = ft_strdup("<<");
			minishell->arg = 0;
			break ;
		}
		else if (argline[i] == '>' && argline[i + 1] && argline[i + 1] == '>')
		{
			printf("HUH?\n");
			minishell->cmd = ft_strdup(">>");
			minishell->arg = 0;
			break ;
		}
		else if (argline[i] == '<')
		{
			minishell->cmd = ft_strdup("<");
			minishell->arg = 0;
			break ;
		}
		else if (argline[i] == '>')
		{
			minishell->cmd = ft_strdup(">");
			minishell->arg = 0;
			break ;
		}
	}
	printf("REDIR CMD: %s\n", minishell->cmd);
}
