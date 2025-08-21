/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_find_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:04:37 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:27:27 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*search_in_dirs(char *cmd, char *path_env)
{
	char	**dirs;
	char	*full_path;
	int		i;

	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = -1;
	while (dirs[++i])
	{
		full_path = strjoin_three(dirs[i], "/", cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_split(dirs);
			return (full_path);
		}
		if (full_path)
			free(full_path);
	}
	ft_free_split(dirs);
	return (NULL);
}

static char	*handle_no_path_env(char *cmd, t_shell *shell)
{
	char	*path;
	char	*cwd;

	if (shell->noenvpath && shell->noenvpath[0] != '\0')
		return (search_in_dirs(cmd, shell->noenvpath));
	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		path = strjoin_three(cwd, "/", cmd);
		free(cwd);
		if (access(path, X_OK) == 0)
			return (path);
		if (path)
			free(path);
	}
	return (NULL);
}

char	*find_in_path(char *cmd, t_exec_data *data)
{
	char	*path;
	char	*full_path;

	if (!cmd || !*cmd)
		return (NULL);
	if (cmd[0] == '~')
		return (ft_strdup(get_env_value("HOME", data->shell->env)));
	if (!ft_strcmp(cmd, ".."))
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		fprintf(stderr, "ellibash: '%s': dosya yada dizin bulunamadı\n", cmd);
		ft_cleaner(data);
		exit(127);
	}
	path = get_env_value("PATH", data->shell->env);
	if (!path)
		return (handle_no_path_env(cmd, data->shell));
	full_path = search_in_dirs(cmd, path);
	return (full_path);
}
