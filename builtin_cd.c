/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlampio <jlampio@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/23 07:30:36 by jlampio           #+#    #+#             */
/*   Updated: 2024/09/23 16:56:02 by jlampio          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int cd_to_oldpwd(char *oldpwd)
{
    if (!oldpwd)
    {
        ft_putstr_fd("cd: OLDPWD not set\n", STDOUT_FILENO);
        return (1);
    }
    if (chdir(oldpwd) != 0)
    {
        perror("cd");
        return (1);
    }
    ft_putstr_fd(oldpwd, STDOUT_FILENO);
    ft_putstr_fd("\n", STDOUT_FILENO);
    return (0);
}

static int cd_check_args(char **args)
{
    if (args[1])
    {
        ft_putstr_fd("cd: too many arguments\n", STDOUT_FILENO);
        return (1);
    }
    return (0);
}

static int cd_to_home(char *home)
{
    if (!home)
    {
        ft_putstr_fd("cd: HOME not set\n", STDOUT_FILENO);
        return (1);
    }
    if (chdir(home) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}


static int cd_to_path(char *path)
{
    if (chdir(path) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}

int builtin_cd(char **args, t_env *env)
{
    char *oldpwd;
    char *home;
    char *pwd;
    char cwd[1024];

 	oldpwd = get_env_value(env, "OLDPWD");
	home = get_env_value(env, "HOME");
	pwd = get_env_value(env, "PWD");
    if (cd_check_args(args))
        return (1);
    if (!args[0])
        if (cd_to_home(home) == 1)
            return (1);
    else if (ft_strcmp(args[0], "-") == 0)
        if (cd_to_oldpwd(oldpwd) == 1)
            return (1);
    else
        if (cd_to_path(args[0]) == 1)
            return (1);
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        update_environment_variable(env, "OLDPWD", pwd);
    return (0);
}


// int builtin_cd(char **args, t_env *env)
// {
//     char	*oldpwd; 
//     char	*home;
// 	char	*pwd;
//     char	cwd[1024];

// 	oldpwd = get_env_value(env, "OLDPWD");
// 	home = get_env_value(env, "HOME");
// 	pwd = get_env_value(env, "PWD");
//     if (args[1])
//     {
//         ft_putstr_fd("cd: too many arguments\n", STDOUT_FILENO);
//         return (1);
//     }
//     if (!args[0])
//     {
//         if (!home)
//         {
//             ft_putstr_fd("cd: HOME not set\n", STDOUT_FILENO);
//             return (1);
//         }
//         if (chdir(home) != 0)
//             perror("cd");
//     }
//     else if (ft_strcmp(args[0], "-") == 0)
//     {
//         if (!oldpwd)
//         {
//             ft_putstr_fd("cd: OLDPWD not set\n", STDOUT_FILENO);
//             return (1);
//         }
//         if (chdir(oldpwd) != 0)
//         {
//             perror("cd");
//             return (1);
//         }
//         ft_putstr_fd(oldpwd, STDOUT_FILENO);
//         ft_putstr_fd("\n", STDOUT_FILENO);
//     }
//     else
//     {
//         if (chdir(args[0]) != 0)
//         {
//             perror("cd");
//             return (1);
//         }
//     }
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
// 		update_environment_variable(env, "OLDPWD", pwd);
// 	return (0);
// }
