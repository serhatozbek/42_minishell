/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:03:18 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:26:33 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

static void	sort_env_copy(char **env_copy, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - i - 1)
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				temp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = temp;
			}
		}
	}
}

static void	print_env_var_formatted(char *env_var)
{
	char	*eq_ptr;

	if (ft_strncmp(env_var, "_=", 2) == 0)
		return ;
	eq_ptr = ft_strchr(env_var, '=');
	write(STDOUT_FILENO, "declare -x ", 11);
	if (eq_ptr)
	{
		write(STDOUT_FILENO, env_var, eq_ptr - env_var);
		write(STDOUT_FILENO, "=\"", 2);
		write(STDOUT_FILENO, eq_ptr + 1, ft_strlen(eq_ptr + 1));
		write(STDOUT_FILENO, "\"\n", 2);
	}
	else
	{
		write(STDOUT_FILENO, env_var, ft_strlen(env_var));
		write(STDOUT_FILENO, "\n", 1);
	}
}

int	print_sorted_env(t_shell *shell)
{
	char	**env_copy;
	int		i;
	int		count;

	count = 0;
	while (shell->env && shell->env[count])
		count++;
	env_copy = malloc(sizeof(char *) * (count + 1));
	if (!env_copy)
		return (1);
	i = -1;
	while (shell->env && shell->env[++i])
		env_copy[i] = shell->env[i];
	env_copy[i] = NULL;
	sort_env_copy(env_copy, count);
	i = -1;
	while (env_copy[++i])
		print_env_var_formatted(env_copy[i]);
	free(env_copy);
	return (0);
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	set_env(t_shell *shell, char *assignment)
{
	char	*eq_ptr;
	int		len;

	eq_ptr = ft_strchr(assignment, '=');
	if (!eq_ptr)
		return (0);
	len = eq_ptr - assignment;
	if (update_existing_env(shell, assignment, len))
		return (0);
	return (add_new_env(shell, assignment));
}
