/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:04:21 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:04:23 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

static void	script_permission_check(char *path, t_exec_data *data)
{
	fprintf(stderr, "ellibash: %s : permissions denied\n", path);
	ft_cleaner(data);
	free(path);
	exit(126);
}

static void	execute_as_script(char *path, char **args, t_exec_data *data)
{
	char	**new_args;
	int		i;

	if (access(path, R_OK) != 0)
		script_permission_check(path, data);
	i = 0;
	while (args[i])
		i++;
	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		exit(126);
	new_args[0] = "sh";
	i = 0;
	while (args[i])
	{
		new_args[i + 1] = args[i];
		i++;
	}
	free(path);
	new_args[i + 1] = NULL;
	execve("/bin/sh", new_args, data->shell->env);
	perror("ellibash : execve (sh)");
	free(new_args);
	ft_cleaner(data);
	exit(126);
}

void	exec_cmd(t_cmd *cmd, t_exec_data *data)
{
	char		*path;
	struct stat	path_stat;

	path = find_in_path(cmd->args[0], data);
	if (!path)
	{
		fprintf(stderr, "ellibash: '%s': command not found\n", cmd->args[0]);
		ft_cleaner(data);
		exit(127);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		fprintf(stderr, "ellibash: %s: is a directory\n", path);
		free(path);
		ft_cleaner(data);
		exit(126);
	}
	execve(path, cmd->args, data->shell->env);
	if (errno != EACCES)
		execute_as_script(path, cmd->args, data);
	perror("ellibash : execve");
	free(path);
	ft_cleaner(data);
	exit(126);
}
