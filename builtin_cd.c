/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:30:36 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 08:05:22 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void builtin_cd(char **args, t_env *env)
{
    char	*oldpwd = get_env_value(env, "OLDPWD");
    char	*home = get_env_value(env, "HOME");
	char	*pwd = get_env_value(env, "PWD");
    char	cwd[1024];

    if (args[1])
    {
        ft_putstr_fd("cd: too many arguments\n", STDOUT_FILENO);
        return;
    }
    if (!args[0])
    {
        if (!home)
        {
            ft_putstr_fd("cd: HOME not set\n", STDOUT_FILENO);
            return;
        }
        if (chdir(home) != 0)
            perror("cd");
    }
    else if (ft_strcmp(args[0], "-") == 0)
    {
        if (!oldpwd)
        {
            ft_putstr_fd("cd: OLDPWD not set\n", STDOUT_FILENO);
            return;
        }
        if (chdir(oldpwd) != 0)
        {
            perror("cd");
            return;
        }
        ft_putstr_fd(oldpwd, STDOUT_FILENO);
        ft_putstr_fd("\n", STDOUT_FILENO);
    }
    else
    {
        if (chdir(args[0]) != 0)
        {
            perror("cd");
            return;
        }
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_environment_variable(env, "OLDPWD", pwd);
}
