/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset_env_exit.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:04:01 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:26:55 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	i = 1;
	if (args[1] && args[1][0] == '-')
	{
		fprintf(stderr, "bash: unset: -T: invalid option\n");
		return (2);
	}
	while (args[i])
	{
		if (!ft_strcmp(args[i], "PATH") && shell->noenvpath != NULL)
		{
			free(shell->noenvpath);
			shell->noenvpath = ft_strdup("");
		}
		unset_env(shell, args[i]);
		i++;
	}
	return (0);
}

int	builtin_env(t_shell *shell)
{
	int	i;
	int	c;

	c = 0;
	if (!shell || !shell->env)
		return (1);
	i = 0;
	while (shell->env[i])
	{
		c = write(STDOUT_FILENO, shell->env[i], ft_strlen(shell->env[i]));
		if (c == -1)
			return (c);
		c = write(STDOUT_FILENO, "\n", 1);
		if (c == -1)
			return (c);
		i++;
	}
	if (c == -1)
		return (c);
	return (0);
}

static int	exit_error(char **str, t_shell *shell, int child_flag)
{
	shell->exit_status = 2;
	if (child_flag == 1)
	{
		fprintf(stderr, "ellibash: exit: %s: numeric argument required\n",
			str[1]);
		return (2);
	}
	write(STDOUT_FILENO, "exit\n", 5);
	fprintf(stderr, "ellibash: exit: %s: numeric argument required\n", str[1]);
	return (0);
}

static int	is_numeric_string(char **str, t_shell *shell, int child_flag)
{
	int	i;

	i = 0;
	if (str[1][i] == '+' || str[1][i] == '-')
		i++;
	if (str[1][i] == '\0')
		return (exit_error(str, shell, child_flag));
	while (str[1][i])
	{
		if (str[1][i] < '0' || str[1][i] > '9')
			return (exit_error(str, shell, child_flag));
		i++;
	}
	if (str[2])
	{
		write(STDOUT_FILENO, "exit\n", 5);
		fprintf(stderr, "ellibash: exit: too many arguments\n");
		return (3);
	}
	return (1);
}

int	builtin_exit(char **args, t_shell *shell, int child_flag)
{
	long long	status;

	status = 0;
	if (!args[1])
	{
		write(STDOUT_FILENO, "exit\n", 5);
		if (child_flag == 1)
			return (0);
		return (EXIT_SHELL);
	}
	status = is_numeric_string(args, shell, child_flag);
	if (status == 3)
		return (1);
	if (status == 2)
		return (2);
	if (status == 0)
		return (EXIT_SHELL);
	status = ft_atoi(args[1]);
	shell->exit_status = (unsigned char)status;
	if (child_flag == 1)
		return (status);
	return (EXIT_SHELL);
}
