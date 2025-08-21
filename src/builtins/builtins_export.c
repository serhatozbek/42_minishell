/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:03:34 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:03:37 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <stdio.h>

static void	handle_append_export(t_shell *shell, char *key, char *value)
{
	char	*old_value;
	char	*new_value;
	char	*full_assignment;
	char	*temp;

	old_value = get_env_value(key, shell->env);
	if (old_value)
		new_value = ft_strjoin(old_value, value);
	else
		new_value = ft_strdup(value);
	temp = ft_strjoin(key, "=");
	full_assignment = ft_strjoin(temp, new_value);
	free(temp);
	set_env(shell, full_assignment);
	free(new_value);
	free(full_assignment);
}

static int	handle_invalid_option(char *arg)
{
	fprintf(stderr, "ellibash: export: `%s': invalid option\n", arg);
	return (2);
}

static int	handle_invalid_identifier(char *arg, int *ret_status)
{
	fprintf(stderr, "ellibash: export: `%s': not a valid identifier\n", arg);
	*ret_status = 1;
	return (0);
}

static void	process_export_variable(t_shell *shell, char *arg, int *ret_status)
{
	char	*key;
	char	*value_ptr;

	value_ptr = ft_strchr(arg, '=');
	if (ft_strnstr(arg, "+=", ft_strlen(arg)) == value_ptr - 1)
		key = ft_substr(arg, 0, value_ptr - arg - 1);
	else if (value_ptr)
		key = ft_substr(arg, 0, value_ptr - arg);
	else
		key = ft_strdup(arg);
	if (!is_valid_identifier(key))
		handle_invalid_identifier(arg, ret_status);
	else if (ft_strnstr(arg, "+=", ft_strlen(arg)) == value_ptr - 1)
		handle_append_export(shell, key, value_ptr + 1);
	else if (value_ptr)
		set_env(shell, arg);
	free(key);
}

int	builtin_export(char **args, t_shell *shell)
{
	int	i;
	int	ret_status;

	ret_status = 0;
	if (!args[1])
		return (print_sorted_env(shell));
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1] != '\0')
			return (handle_invalid_option(args[i]));
		process_export_variable(shell, args[i], &ret_status);
		i++;
	}
	return (ret_status);
}
