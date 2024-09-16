/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 10:36:37 by alogvine          #+#    #+#             */
/*   Updated: 2024/09/16 16:15:04 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*de_quote(char *line, t_env *env);

/*t_cmds	*create_cmd(char *pipeline, t_env *env)
{
	t_cmds	*new;
	char	*temp;
	char	*arg;
	char	**argline;
	int		i;

	new = malloc(sizeof(t_cmds));
	if (!new)
		return (0);
	argline = ft_split(pipeline, ' ');
	arg = ft_strdup("");
	new->cmd = ft_strdup(de_quote(argline[0], env));
	if (!new->cmd[0])
		return (0);
	i = 1;
	while (argline[i])
	{

		temp = arg;
		arg = ft_strjoin(arg, argline[i++]);
		if (argline[i])
			arg = ft_strjoin(arg, " ");
		free(temp);
	}
	new->arg = de_quote(arg, env);
	new->next = 0;
	return (new);
}*/

t_args	*create_arg(char *arg, t_env *env)
{
	t_args	*new;

	new = malloc(sizeof(t_args));
	if (!new)
		return (0);
	new->arg = ft_strdup(de_quote(arg, env));
	new->next = 0;
	return (new);
}

t_args	*create_args(char **args, t_env *env)
{
	t_args	*targs;
	t_args	*new;
	t_args	*curr;
	int		i;

	i = 1;
	while (args[i])
	{
		new = create_arg(args[i], env);
		if (!new)
			return (0);
		if (i == 1)
			targs = new;
		else if (curr)
			curr->next = new;
		curr = new;
		i++;
	}
	curr->next = 0;
	return (targs);
}

t_cmds	*create_cmd(char *pipeline, t_env *env)
{
	t_cmds	*new;
	char	**args;

	new = malloc(sizeof(t_cmds));
	if (!new)
		return (0);
	args = pipesplit(pipeline, ' ');
	if (!args)
		return (0);
	new->cmd = ft_strdup(args[0]);
	new->args = create_args(args, env);
	return (new);
}

t_cmds	*create_cmds(char *pipeline, t_minishell *minishell)
{
	t_cmds	*current;
	t_cmds	*new;

	current = minishell->cmds;
	if (pipeline)
	{
		new = create_cmd(pipeline, minishell->env);
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
	current->next = 0;
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

char	*ft_cjoin(char *s1, char const c)
{
	size_t	i;
	size_t	n;
	char	*str;

	if (!s1 || !c)
		return (0);
	i = 0;
	n = 0;
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
	char	*str;
	char	*key;

	str = ft_strdup("");
	key = ft_strdup("");
	if (*line == '$')
	{
		if (*line == '$' && *line + 1 == '"')
			str = ft_cjoin(new, '$');
		else if (*line++)
		{
			str = ft_strjoin(new, str);
			while (*line && *line != ' ' && *line != '"')
				key = ft_cjoin(key, *line++);
			while (env)
			{
				if (!ft_strcmp(key, env->key))
					str = ft_strjoin(str, env->value);
				env = env->next;
			}
		}
	}
	else
		str = ft_cjoin(new, *line);
	free(key);
	return (str);
}

char	*de_quote(char *line, t_env *env)
{
	int		i;
	char	*new;

	i = 0;
	new = ft_strdup("");
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			if (check_end_of_quotes(line + i))
			{
				ft_putstr_fd("GIT GUD!\n", 1);
				free(line);
				return ("");
			}
			if (line[i++] == '\'')
				while (line[i] != '\'')
					new = ft_cjoin(new, line[i++]);
			else
			{
				while (line[i] && line[i] != '"')
				{
					if (line[i] == '$')
					{
						new = ft_expjoin(new, line + i, env);
						while (line[i] && line[i] != ' ' && line[i] != '"')
							i++;
						if (line[i] == '"')
							i++;
					}
					else
						new = ft_expjoin(new, line + i++, env);
				}
			}
			i++;
		}
		else
		{
			new = ft_expjoin(new, line + i, env);
			if (line[i] && line[i] == '$')
				while (line[i] && line[i] != ' ')
					i++;
			else
				i++;
		}
	}
	free(line);
	return (new);
}

int	add_to_structs(t_minishell *minishell, char *line)
{
	int		i;
	int		n;
	char	**pipeline;

	i = 0;
	n = 0;
	if (!line)
		return (1);
	pipeline = pipesplit(line, '|');
	while (pipeline[n])
	{
		printf("PIPE AFTER SPLIT: %s\n", pipeline[n]);
		while (line[i] && !(line[i] == '<' || line[i] == '>'))
			i++;
		if (line[i] && (line[i] == '<' || line[i] == '>'))
			ft_putstr_fd("FOUND REDIR!!!\n", 1);
		else
			minishell->cmds = create_cmds(pipeline[n++], minishell);
	}
//	while (minishell->cmds)
//	{
//		printf("COMMAND: %s\nARGUMENT:\n", minishell->cmds->cmd);//, minishell->cmds->args->arg);
//		minishell->cmds = minishell->cmds->next;
//	}
	i = 0;
	while (pipeline[i])
		free(pipeline[i++]);
	free(pipeline);
	return (0);
}

void	do_command(t_minishell *minishell)
{
	check_builtins(minishell);
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
	while (envp[i])
	{
		str = ft_split(envp[i], '=');
		if (!str)
			return (0);
		new = create_node(str[0], str[1]);
		free(str);
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
	ft_bzero(minishell, sizeof(t_minishell));
	minishell->env = init_env(envp);
	minishell->cmds = 0;
	minishell->pipe = 0;
	return (minishell);
}

int	check_quotes(char *line)
{
	int	i;

	i = 0;
	if (line[i] && line[i] == '\'')
	{
		i++;
		while (line[i] && line[i] != '\'')
			i++;
	}
	else if (line[i] && line[i] == '"')
	{
		i++;
		while (line[i] && line[i] != '"')
			i++;
	}
	return (i);
}

int	check_double(char *line)
{
	int	i;

	i = 0;
	while (line[i++])
	{
		i += check_quotes(line + i);
		if (line[i] && line[i] == '|')
		{
			if (line[i++ + 1] == '|')
				return (1);
			if (line[i] == '&')
				i++;
			while (line[i] && line[i] == ' ')
				i++;
			if (line[i] && (line[i] == '|'))
				return (2);
		}
		else if (line[i] && line [i] == '&')
		{
			if (line[i++ + 1] == '&')
				return (1);
			while (line[i] && line[i] == ' ')
				i++;
			if (line[i] && (line[i] == '&' || line[i] == '|'))
				return (2);
		}
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
	if (!check_empty(line))
		return (1);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	char		*line;
	t_minishell	*minishell;

	(void)av;
	if (ac == 1)
	{
		minishell = init_minishell(envp);
		while (1)
		{
			line = readline("minishell> ");
			add_history(line);
			if (!line || !*line || check_syntax(line))
			{
				free(line);
				continue ;
			}
			if (add_to_structs(minishell, line))
				continue ;
			do_command(minishell);
			free_cmds(minishell->cmds);
			minishell->cmds = 0;
		}
	}
	return (0);
}
