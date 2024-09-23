/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 14:45:07 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 14:48:35 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char *construct_full_path(char *dir, char *cmd)
{
    char full_path[1024];

    full_path[0] = '\0';
    ft_strlcat(full_path, dir, sizeof(full_path));
    ft_strlcat(full_path, "/", sizeof(full_path));
    ft_strlcat(full_path, cmd, sizeof(full_path));

    if (access(full_path, X_OK) == 0)
        return ft_strdup(full_path);

    return NULL;
}

char *find_executable(char *cmd, t_env *env_list)
{
    int i;
    char **dir;
    char *path;

    if (cmd[0] == '/' || ft_strncmp(cmd, "./", 2) == 0 || ft_strncmp(cmd, "../", 3) == 0)
        return (access(cmd, X_OK) == 0 ? ft_strdup(cmd) : NULL);
    path = get_env_value(env_list, "PATH");
    if (!path)
        return NULL;
    dir = ft_split(path, ':');
    i = 0;
    while (dir[i])
    {
        char *full_path = construct_full_path(dir[i], cmd);
        if (full_path)
        {
            free(path);
            freestr(dir);
            return full_path;
        }
        i++;
    }
    free(path);
    freestr(dir);
    return NULL;
}

int is_builtin(char *cmd)
{
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "pwd") ||
            !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "exit"));
}

void safe_exit(t_minishell *minishell, int status)
{
	if (minishell->pid)
	{
    	free(minishell->pid);
    	minishell->pid = NULL;
	}
    exit(status);
}
