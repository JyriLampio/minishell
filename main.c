/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:36:37 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/19 18:48:54 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_args	*create_arg(char *arg, t_minishell *minishell)
{
	t_args	*new;

	new = malloc(sizeof(t_args));
	if (!new)
		return (0);
	new->arg = ft_strdup(expand(arg, minishell));
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

t_cmds	*create_cmd(char *pipeline, t_minishell *minishell, int pipe)
{
	t_cmds	*new;
	char	**args;

	new = malloc(sizeof(t_cmds));
	if (!new)
		return (0);
	args = pipesplit(pipeline, ' ');
	if (!args)
		return (0);
	new->args = 0;
	new->cmd = ft_strdup(expand(*args++, minishell));
	while (*args)
		new->args = create_args(new->args, *args++, minishell);
	new->next = 0;
	new->pipe = pipe;
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

int	check_end_of_quotes(char *line)
{
	int		i;
	char	quote;

	i = 0;
	quote = line[i];
	i++;
	while (line[i])
	{
		if (line[i] == quote)
			return (0);
		i++;
	}
	return (1);
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
	return (str);
}

char	*ft_expjoin(char *new, char *line, t_env *env)
{
	char	*str;
	char	*key;

	str = ft_strdup("");
	key = ft_strdup("");
	if (*line == '$')
	{
		line++;
		str = ft_strjoin(str, new);
		while (*line && *line != ' ' && *line != '"')
			key = ft_cjoin(key, *line++);
		while (env)
		{
			if (!ft_strcmp(key, env->key))
				str = ft_strjoin(str, env->value);
			env = env->next;
		}
	}
	else
		str = ft_cjoin(new, *line);
	free(key);
	return (str);
}

char	*ft_expqjoin(char *new, char *line, t_env *env)
{
	char	*key;

	key = ft_strdup("");
	line += 2;
	while (*line && *line != ' ' && *line != '"')
		key = ft_cjoin(key, *line++);
	while (env)
	{
		if (!ft_strcmp(key, env->key))
			new = ft_strjoin(new, env->value);
		env = env->next;
	}
	free(key);
	return (new);
}

char	*qjoin(char	*str)
{
	char	*new;

	new = ft_strdup("");
	new = ft_cjoin(new, *str++);
	while (*str != '\'')
		new = ft_cjoin(new, *str++);
	new = ft_cjoin(new, *str);
	return (new);
}

char	*qexpand(char *line, t_minishell *minishell)
{
	char	*new;

	new = ft_strdup("");
	line++;
	while (new && *line && *line != '"')
	{
		if (*line == '$' && (*(line + 1) == '"' || *(line + 1) == ' ' || !*(line + 1)))
			new = ft_cjoin(new, *line++);
		else if (*line == '$' && *(line + 1) == '?')
			printf("???????????????\n");
		else if (*line == '$' && *(line + 1) == '\'')
			new = ft_cjoin(new, *line);
		else if (*line == '$')
		{
			new = ft_expjoin(new, line, minishell->env);
			line += qlen(line, ' ') - 1;
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

char	*expandx(char *line, t_minishell *minishell)
{
	char	*new;

	new = ft_strdup("");
	if (new && *line == '$' && *(line + 1) && *(line + 1) == ' ')
		new = ft_cjoin(new, *line++);
	else if ((*(line + 1) && *(line + 1) == '?')
		|| (*(line + 1) == '"' && *(line + 2) == '?'))
		printf("??????????????\n");
	else if (*(line + 1) && *(line + 1) == '"')
		new = ft_expqjoin(new, line, minishell->env);
	else
		new = ft_expjoin(new, line++, minishell->env);
	return (new);
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
			new = ft_strjoin(new, qexpand(line, minishell));
			line += qlen(line, '"');
		}
		else if (*line == '$' && *(line + 1) && *(line + 1) != '\'' && *(line + 1) != ' ')
		{
			new = ft_strjoin(new, expandx(line, minishell));
			line += qlen(line, ' ') - 1;
		}
		else if (*line == '$' && *(line + 1) == '\'')
			new = ft_cjoin(new, *line++);
		else
			new = ft_cjoin(new, *line++);
	}
	return (new);
}

int	add_to_structs(char *line, t_minishell *minishell)
{
	int		i;
	int		n;
	char	**pipeline;

	i = 0;
	n = 0;
	pipeline = pipesplit(line, '|');
	if (!pipeline || !*pipeline)
		return (1);
	while (pipeline[n])
	{
		if (check_redirs(pipeline[n]))
		{
			if (parse_redirs(pipeline[n++], minishell))
				return (1);
			printf("DO NOT PRINT THIS!!!!!!\n");
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
	while (*line)
	{
		while (*line == '\'' || *line == '"')
			line += qlen(line, *line);
		if (*line && (*line == '|' || *line == '&'))
		{
			if (*line == '|' && *(line + 1) == '|')
				return (1);
			else if (*line == '&' && *(line + 1) == '&')
				return (1);
			line++;
			while (*line && *line == ' ')
				line++;
			if (*line && (*line == '|' || *line == '&'))
				return (2);
		}
		else
			line++;
	}
	return (0);
}

int	check_empty(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int	check_syntax(char *line)
{
	int	t;

	if (!check_empty(line))
		return (1);
	if (check_quotes(line))
		t = 2;
	else
		t = check_double(line);
	if (t > 0)
	{
		if (t == 1)
			ft_putstr_fd("Command not avalaible, \
please purchase premium for $420,69\n", 1);
		if (t == 2)
			ft_putstr_fd("Syntax error\n", 1);
		return (1);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_minishell	*minishell;

	(void)ac;
	(void)av;
	minishell = init_minishell(envp);
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
			printf("Malloc failed xd\n");
		if (do_command(minishell))
			return (0);
		freecmds(minishell->cmds);
		minishell->cmds = 0;
		free(line);
	}
	return (0);
}
