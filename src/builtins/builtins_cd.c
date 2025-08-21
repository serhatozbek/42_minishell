/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:03:02 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:26:23 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void	update_pwd_env(t_shell *shell)
{
	char	*cwd;
	char	*pwd_assignment;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	pwd_assignment = ft_strjoin("PWD=", cwd);
	free(cwd);
	if (!pwd_assignment)
		return ;
	set_env(shell, pwd_assignment);
	free(pwd_assignment);
}

static void	update_oldpwd_env(t_shell *shell, char *old_pwd)
{
	char	*oldpwd_assignment;

	if (old_pwd)
	{
		oldpwd_assignment = ft_strjoin("OLDPWD=", old_pwd);
		free(old_pwd);
		if (!oldpwd_assignment)
			return ;
		set_env(shell, oldpwd_assignment);
		free(oldpwd_assignment);
	}
}

static char	*get_special_cd_path(char **args, t_shell *shell)
{
	char	*path;

	if (ft_strcmp(args[1], "-") == 0)
	{
		if (args[2])
		{
			fprintf(stderr, "ellibash: cd: too many arguments\n");
			shell->exit_status = 1;
			return (NULL);
		}
		path = get_env_value("OLDPWD", shell->env);
		if (!path || *path == '\0')
		{
			fprintf(stderr, "ellibash: cd: OLDPWD not set\n");
			return (NULL);
		}
		return (path);
	}
	return (NULL);
}

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1] || ft_strcmp(args[1], "~") == 0
		|| (ft_strcmp(args[1], "--") == 0 && !args[2]))
	{
		path = get_env_value("HOME", shell->env);
		if (!path || *path == '\0')
		{
			fprintf(stderr, "ellibash: cd: HOME not set\n");
			return (NULL);
		}
		return (path);
	}
	path = get_special_cd_path(args, shell);
	if (path)
		return (path);
	return (args[1]);
}

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		perror("ellibash: cd: getcwd\n");
		return (1);
	}
	path = get_cd_path(args, shell);
	if (!path)
	{
		free(old_pwd);
		return (1);
	}
	if (chdir(path) != 0)
	{
		free(old_pwd);
		fprintf(stderr, "ellibash: cd: %s: %s\n", path, strerror(errno));
		return (1);
	}
	update_oldpwd_env(shell, old_pwd);
	update_pwd_env(shell);
	return (0);
}
