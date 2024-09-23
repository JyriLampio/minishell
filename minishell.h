/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:33:14 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/23 18:15:28 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>

# define MAX_PATH_LENGTH 1024

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC,
	REDIR_APPEND
}		t_redir_type;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}		t_env;

typedef struct s_redirs
{
	t_redir_type	type;
	char			*file;
	struct s_redirs	*next;
}		t_redirs;

typedef struct s_args
{
	char			*arg;
	struct s_args	*next;
}		t_args;

typedef struct s_cmds
{
	char			*cmd;
	t_args			*args;
	t_redirs		*redirs;
	struct s_cmds	*next;
}		t_cmds;

// typedef struct s_minishell
// {
// 	t_env	*env;
// 	t_cmds	*cmds;
// }		t_minishell;

typedef struct s_minishell
{
	t_env	*env;
	t_cmds	*cmds;
	int		pipefds[2];
	int		num_cmds;
	int		exit_status;
	int		*pid;
	int		write_end;
}		t_minishell;

char			**ft_split(char const *s, char c);
size_t			ft_strlen(const char *str);
void			*ft_calloc(size_t count, size_t size);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char *s1, char *s2);
void			*ft_memset(void *b, int i, size_t n);
char			*ft_strdup(const char *s);
int				ft_strcmp(const char *s1, const char *s2);
int				check_builtins(t_minishell *minishell);
void			ft_putstr_fd(char *s, int fd);
char			*ft_strchr(const char *s, int i);
void			parsing_redirs(t_minishell *minishell, char *argline);
void			ft_bzero(void *s, size_t n);
void			freecmds(t_cmds *cmds);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
char			**pipesplit(char *line, char c);
char			**freestr(char **str);
char			*expand(char *str, t_minishell *minishell);
int				qlen(char *line, char c);
int				check_redirs(char *line);
char			*parse_redirs(char *line);
void			freeminishell(t_minishell *minishell);
void			freeenv(t_env *env);
char			*ft_cjoin(char *s1, char c);
int				redir_syntax(char *s);
int				dlen(char *line);
int				ft_atoi(char *str);
char			*ft_itoa(int i);
void			*ft_calloc(size_t count, size_t size);
int				check_arrow_file(char *s);
t_minishell		*init_minishell(char **envp);
int				check_syntax(char *line);
t_cmds			*make_cmds(char *pipeline, t_minishell *minishell, int pipe);
t_redirs		*make_redir(char *redir, char *file, t_minishell *minishell);
char			*expand(char *line, t_minishell *minishell);
char			*expjoin(char *new, char *line, t_env *env);
char			*cjoin(char *s1, char c);
int				skip_arrow(char *line);

// execve and builtins
char			*get_env_value(t_env *env_list, const char *key);
t_env			*ft_lstlast(t_env *lst);
void			free_split(char **split);
// void	check_builtins(t_minishell *minishell);
int				is_builtin(char *cmd);
int				execute_builtin(t_minishell *minishell, t_cmds *current_cmd);
int				pipe_x(t_minishell *minishell);
int				wait_processes(int pid);
int				handle_redirections(t_cmds *cmd);
int				builtin_cd(char **args, t_env *env);
void			update_environment_variable(t_env *env, char *key, char *value);
char			**arr_args(t_args *args);
char			**split_on_first_equals(char *str);
char			**build_argv(char *cmd_path, t_args *args);
int				prepare_execution(t_minishell *minishell);
int				error_msg(t_minishell *minishell, char *file, int exit_code);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				is_builtin(char *cmd);
void			safe_exit(t_minishell *minishell, int status, char **envp);
char			*find_executable(char *cmd, t_env *env_list);
int				execute_cmds(t_minishell *minishell, char **envp);
void			execute_child_process(t_minishell *minishell,
					t_cmds *current_cmd, char **envp, int index);
void			builtin_export(t_env *env, t_args *args);
int				get_cwd(void);
void			print_env(t_env *env);
void			builtin_unset(t_env **env, t_args *args);

// Signal handling
void			handle_parent_signals(int sig);

// Duplication functions
int				dup_stdin(t_minishell *minishell, t_cmds *cmd);
int				dup_stdout(t_minishell *minishell, t_cmds *cmd, int index);
int				dup_file_descriptors(t_minishell *minishell,
					t_cmds *cmd, int index);
void			close_fds(t_minishell *minishell);

// redir functions
int				handle_redirections(t_cmds *cmd);
int				has_input_redirection(t_redirs *redirs);
int				has_output_redirection(t_redirs *redirs);
int				handle_redir_heredoc(t_redirs *redir);
int				handle_redir_append(t_redirs *redir);

#endif
