/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:33:14 by alogvine          #+#    #+#             */
/*   Updated: 2024/08/19 12:31:37 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_minishell
{
	char *cmd;
	char *arg;
}		t_minishell;

char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *str);
void    *ft_calloc(size_t count, size_t size);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char    *ft_strjoin(char *s1, char *s2);
void	*ft_memset(void *b, int i, size_t n);
char	*ft_strdup(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
void    check_builtins(t_minishell *minishell, int i);

#endif
