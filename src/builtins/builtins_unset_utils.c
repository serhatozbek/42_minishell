/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:04:10 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:27:03 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdlib.h>

static char	**create_new_env_without_var(char **old_env, int count,
		int remove_idx)
{
	char	**new_env;
	int		i;
	int		j;

	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < count)
	{
		if (i != remove_idx)
			new_env[j++] = old_env[i];
		else
			free(old_env[i]);
	}
	new_env[j] = NULL;
	free(old_env);
	return (new_env);
}

static int	find_env_var_index(char **env, const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && (env[i][len] == '='
			|| env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

int	update_existing_env(t_shell *shell, char *assignment, int len)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], assignment, len) == 0
			&& (shell->env[i][len] == '=' || shell->env[i][len] == '\0'))
		{
			free(shell->env[i]);
			shell->env[i] = ft_strdup(assignment);
			return (1);
		}
		i++;
	}
	return (0);
}

int	add_new_env(t_shell *shell, char *assignment)
{
	int		i;
	int		count;
	char	**new_env;

	count = 0;
	while (shell->env && shell->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = -1;
	while (++i < count)
		new_env[i] = shell->env[i];
	new_env[i] = ft_strdup(assignment);
	if (!new_env[i])
	{
		free(new_env);
		return (1);
	}
	new_env[i + 1] = NULL;
	if (shell->env)
		free(shell->env);
	shell->env = new_env;
	return (0);
}

int	unset_env(t_shell *shell, char *name)
{
	int	index_to_remove;
	int	count;

	if (!name || !shell || !shell->env || !*name)
		return (0);
	index_to_remove = find_env_var_index(shell->env, name);
	if (index_to_remove == -1)
		return (0);
	count = 0;
	while (shell->env[count])
		count++;
	shell->env = create_new_env_without_var(shell->env, count, index_to_remove);
	if (!shell->env)
		return (1);
	return (0);
}
