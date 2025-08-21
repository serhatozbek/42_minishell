/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_general.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:03:49 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:26:47 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

static int	is_n_option(char *arg)
{
	int	i;

	if (!arg)
		return (0);
	if (arg[0] != '-')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (i > 1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	newline_flag;

	newline_flag = 1;
	i = 1;
	while (args[i] && is_n_option(args[i]))
	{
		newline_flag = 0;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline_flag)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

static int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		free(cwd);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(t_cmd *cmd, t_shell *shell, int child_flag)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (!ft_strcmp(cmd->args[0], "echo"))
		return (builtin_echo(cmd->args));
	else if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args, shell));
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(cmd->args[0], "export"))
		return (builtin_export(cmd->args, shell));
	else if (!ft_strcmp(cmd->args[0], "unset"))
		return (builtin_unset(cmd->args, shell));
	else if (!ft_strcmp(cmd->args[0], "env") && !cmd->args[1])
		return (builtin_env(shell));
	else if (!ft_strcmp(cmd->args[0], "exit"))
		return (builtin_exit(cmd->args, shell, child_flag));
	return (127);
}
