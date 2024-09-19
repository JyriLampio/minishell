/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:33:14 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/19 16:56:43 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}		t_env;

typedef struct s_args
{
	char			*arg;
	struct s_args	*next;
}		t_args;

typedef struct s_cmds
{
	char			*cmd;
	t_args			*args;
	int				pipe;
	struct s_cmds	*next;
}		t_cmds;

typedef struct s_minishell
{
	t_env	*env;
	t_cmds	*cmds;
}		t_minishell;

char	**ft_split(char const *s, char c);
size_t	ft_strlen(const char *str);
void	*ft_calloc(size_t count, size_t size);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
void	*ft_memset(void *b, int i, size_t n);
char	*ft_strdup(const char *s);
int		ft_strcmp(const char *s1, const char *s2);
int		check_builtins(t_minishell *minishell);
void	ft_putstr_fd(char *s, int fd);
char	*ft_strchr(const char *s, int i);
void	parsing_redirs(t_minishell *minishell, char *argline);
void	ft_bzero(void *s, size_t n);
void	freecmds(t_cmds *cmds);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	**pipesplit(char *line, char c);
char	**freestr(char **str);
char	*expand(char *str, t_minishell *minishell);
int		qlen(char *line, char c);
int		check_redirs(char *line);
int		parse_redirs(char *line, t_minishell *minishell);
void	freeminishell(t_minishell *minishell);
void	freeenv(t_env *env);
char	*ft_cjoin(char *s1, char c);

#endif
