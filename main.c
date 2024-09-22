/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:36:37 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/22 12:27:16 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args	*create_arg(char *arg, t_minishell *minishell)
{
	t_args	*new;

	new = malloc(sizeof(t_args));
	if (!new)
		return (0);
	new->arg = expand(arg, minishell);
	new->next = 0;
	return (new);
}

t_args	*create_args(t_args *targ, char *args, t_minishell *minishell)
{
	t_args	*new;
	t_args	*curr;

	curr = targ;
	new = create_arg(args, minishell);
	if (!new)
		return (0);
	if (targ == 0)
		targ = new;
	else if (curr)
	{
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	curr = new;
	return (targ);
}

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

t_redirs	*create_redir(char *redir, char *file, t_minishell *minishell)
{
	t_redirs	*new;

	new = malloc(sizeof(t_redirs));
	if (!new)
		return (0);
	new->type = fetch_redir(redir);
	new->file = expand(file, minishell);
	new->next = 0;
	return (new);
}

t_redirs	*create_redirs(t_redirs *redirs, char *redir,
		char *file, t_minishell *minishell)
{
	t_redirs	*new;
	t_redirs	*curr;

	curr = redirs;
	new = create_redir(redir, file, minishell);
	if (!new)
		return (0);
	if (redirs == 0)
		redirs = new;
	else if (curr)
	{
		while (curr->next)
			curr = curr->next;
		curr->next = new;
	}
	curr = new;
	return (redirs);
}

t_cmds	*create_cmd(char *pipeline, t_minishell *minishell, int redir)
{
	t_cmds	*new;
	char	**args;
	char	**temp;

	new = malloc(sizeof(t_cmds));
	if (!new)
		return (0);
	args = pipesplit(pipeline, ' ');
	if (!args || !*args)
		return (0);
	temp = args;
	new->args = 0;
	new->redirs = 0;
	if (*args && *args[0] != '<' && *args[0] != '>')
		new->cmd = expand(*args++, minishell);
	while (*args && *args[0] != '<' && *args[0] != '>')
		new->args = create_args(new->args, *args++, minishell);
	while (redir == 1 && *args)
	{
		new->redirs = create_redirs(new->redirs, *args, *(args + 1), minishell);
		args += 2;
	}
	new->next = 0;
	freestr(temp);
	return (new);
}

t_cmds	*create_cmds(char *pipeline, t_minishell *minishell, int pipe)
{
	t_cmds	*current;
	t_cmds	*new;

	current = minishell->cmds;
	if (pipeline)
	{
		new = create_cmd(pipeline, minishell, pipe);
		if (!new)
			return (0);
		if (!minishell->cmds)
			minishell->cmds = new;
		else if (current)
		{
			while (current->next)
				current = current->next;
			current->next = new;
		}
		current = new;
	}
	return (minishell->cmds);
}

char	*ft_cjoin(char *s1, char c)
{
	size_t	i;
	char	*str;

	if (!s1 || !c)
		return (0);
	i = 0;
	str = malloc(ft_strlen(s1) + 2);
	if (!str)
		return (0);
	while (s1[i] != 0)
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = c;
	str[i + 1] = 0;
	free(s1);
	return (str);
}

char	*ft_expjoin(char *new, char *line, t_env *env)
{
	char	*key;
	char	*temp;

	temp = new;
	key = ft_strdup("");
	if (!key)
		return (0);
	if (new && *line == '$')
	{
		line++;
		while (key && *line && *line != ' ' && *line != '"' && *line != '\'')
			key = ft_cjoin(key, *line++);
		while (new && key && env)
		{
			if (!ft_strcmp(key, env->key))
				new = ft_strjoin(new, env->value);
			env = env->next;
		}
	}
	else if (key && new)
		new = ft_cjoin(new, *line);
	free(key);
	free(temp);
	return (new);
}

char	*qexpand(char *new, char *line, t_minishell *minishell)
{
	line++;
	while (new && *line && *line != '"')
	{
		if (new && *line == '$' && (*(line + 1) == '"'
				|| *(line + 1) == ' ' || !*(line + 1)))
			new = ft_cjoin(new, *line++);
		else if (new && *line == '$' && *(line + 1) == '?')
			printf("???????????????\n");
		else if (new && *line == '$' && *(line + 1) == '\'')
			new = ft_cjoin(new, *line);
		else if (new && *line == '$')
		{
			new = ft_expjoin(new, line, minishell->env);
			line += dlen(line);
		}
		else
			new = ft_cjoin(new, *line++);
	}
	return (new);
}

char	*noexpand(char *new, char *line)
{
	line++;
	while (new && *line != '\'')
		new = ft_cjoin(new, *line++);
	return (new);
}

char	*dexpand(char *new, char *line, t_minishell *minishell)
{
	if (new && (!*(line + 1) || *(line + 1) == ' '))
		new = ft_cjoin(new, '$');
	else if (new && *(line + 1) && (*(line + 1) == '"' || *(line + 1) == '\''))
		line++;
	else if (new && *(line + 1) && *(line + 1) == '?')
		printf("??????????????\n");
	else if (new && *line)
		new = ft_expjoin(new, line, minishell->env);
	return (new);
}

int	dlen(char *line)
{
	int	i;

	i = 0;
	while (*line && *line != ' ' && *line != '\'' && *line != '"')
	{
		line++;
		i++;
	}
	return (i);
}

char	*expand(char *line, t_minishell *minishell)
{
	char	*new;

	new = ft_strdup("");
	while (new && *line)
	{
		if (*line == '\'')
		{
			new = noexpand(new, line);
			line += qlen(line, *line);
		}
		else if (*line == '"')
		{
			new = qexpand(new, line, minishell);
			line += qlen(line, *line);
		}
		else if (*line == '$')
		{
			new = dexpand(new, line, minishell);
			line += dlen(line);
		}
		else
			new = ft_cjoin(new, *line++);
	}
	return (new);
}

int	add_to_structs(char *line, t_minishell *minishell)
{
	int		n;
	char	**pipeline;
	char	*redirline;

	n = 0;
	pipeline = pipesplit(line, '|');
	if (!pipeline || !*pipeline)
		return (1);
	while (pipeline[n])
	{
		if (check_redirs(pipeline[n]))
		{
			redirline = parse_redirs(pipeline[n++]);
			if (redirline)
				minishell->cmds = create_cmds(redirline, minishell, 1);
			free(redirline);
		}
		else
			minishell->cmds = create_cmds(pipeline[n++], minishell, 0);
	}
	freestr(pipeline);
	return (0);
}

int	do_command(t_minishell *minishell)
{
	if (check_builtins(minishell))
		return (1);
	return (0);
}

int	num_rows(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

t_env	*create_node(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->key = ft_strdup(key);
	new->value = ft_strdup(value);
	new->next = 0;
	return (new);
}

void	set_shlvl(t_env *env)
{
	int		check;

	check = 0;
	while (env)
	{
		if (!ft_strcmp(env->key, "SHLVL"))
		{
			check = 1;
			break ;
		}
		env = env->next;
	}
	if (check)
	{
		free(env->value);
		env->value = ft_itoa(ft_atoi(env->value) + 1);
	}
	else
		env = create_node("SHLVL", "1");
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*current;
	t_env	*new;
	int		i;
	char	**str;

	i = 0;
	while (*envp && envp[i])
	{
		str = ft_split(envp[i], '=');
		if (!str)
			return (0);
		new = create_node(str[0], str[1]);
		freestr(str);
		if (!new)
			return (0);
		if (i == 0)
			env = new;
		else if (current)
			current->next = new;
		current = new;
		i++;
	}
	set_shlvl(env);
	return (env);
}

t_minishell	*init_minishell(char **envp)
{
	t_minishell	*minishell;

	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
	{
		ft_putstr_fd("Error: Malloc fail", 2);
		exit(1);
	}
	minishell->env = init_env(envp);
	minishell->cmds = 0;
	return (minishell);
}

int	check_quotes(char *line)
{
	char	c;

	while (*line)
	{
		if (*line == '\'' || *line == '"')
		{
			c = *line;
			line++;
			while (*line != c)
			{
				line++;
				if (!*line)
					return (1);
			}
		}
		line++;
	}
	return (0);
}

int	check_double(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		while (line[i] && (line[i] == '\'' || line[i] == '"'))
			i += qlen(line, line[i]);
		if (line[i] && (line[i] == '|' || line[i] == '&'))
		{
			if (line[i] == '|' && line[i + 1] == '|')
				return (1);
			else if (line[i] == '&' && line[i + 1] == '&')
				return (1);
			i++;
			while (line[i] && line[i] == ' ')
				i++;
			if (line[i] && (line[i] == '|' || line[i] == '&'))
				return (i);
		}
		else if (line[i])
			i++;
	}
	return (0);
}

int	check_empty(char *line)
{
	while (*line && *line == ' ')
		line++;
	if (!*line)
		return (1);
	else if (*line == '|')
	{
		printf("bobershell: syntax error near unexpected token '%c'\n", *line);
		return (1);
	}
	return (0);
}

int	check_syntax(char *line)
{
	int	t;

	if (check_empty(line))
		return (1);
	if (check_quotes(line))
		t = 2;
	else if (redir_syntax(line))
		t = 2;
	else
		t = check_double(line);
	if (t > 0)
	{
		if (t == 1)
			ft_putstr_fd("Command not available, \
please purchase premium for $420,69\n", 1);
		else if (t == 2)
			ft_putstr_fd("Syntax error\n", 1);
		else if (t > 1)
			printf("bobershell: syntax error \
near unexpected token '%c'\n", line[t]);
		return (1);
	}
	return (0);
}

void	bobershell(t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		line = readline("bobershell> ");
		add_history(line);
		if (!line || !*line || check_syntax(line))
		{
			free(line);
			continue ;
		}
		if (add_to_structs(line, minishell))
			return ;
		if (do_command(minishell))
			return ;
		freecmds(minishell->cmds);
		minishell->cmds = 0;
		free(line);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	*minishell;

	(void)ac;
	(void)av;
	minishell = init_minishell(envp);
	bobershell(minishell);
	freeminishell(minishell);
	return (0);
}
