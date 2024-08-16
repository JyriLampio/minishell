/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:33:14 by alogvine          #+#    #+#             */
/*   Updated: 2024/08/16 12:47:39 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdlib.h>


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

#endif
