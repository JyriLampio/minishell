/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 14:29:48 by alogvine          #+#    #+#             */
/*   Updated: 2024/08/30 17:40:53 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_cmd(t_minishell *minishell, char *argline)
{
	int	i;

	(void)minishell;
	i = 0;
	while (argline[i])
	{
		if (argline[i] == '<' || argline[i] == '>')
			printf("haha\n");
		i++;
	}
}

void	parsing_cmds(t_minishell *minishell, char *argline)
{
	int	i;

	i = 0;
	find_cmd(minishell, argline);
	while (argline[i++])
	{
		if (argline[i] == '<' && argline[i + 1] == '<')
			printf("Found <<\n");
		else if (argline[i] == '>' && argline[i + 1] == '>')
			printf("Found >>\n");
		else if (argline[i] == '<')
			printf("Found <\n");
		else if (argline[i] == '>')
			printf("Found >\n");
		else
			printf("Lol wrong redirection haha\n");
	}
}
