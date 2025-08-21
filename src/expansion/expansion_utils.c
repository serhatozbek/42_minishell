/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:19 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:27:58 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdlib.h>

char	*get_env_value(char *key, char **env)
{
	int	i;
	int	key_len;

	if (!key || !env)
		return (NULL);
	key_len = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], key, key_len) == 0 && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return (NULL);
}

static char	*extract_var_name(char *str, int *i)
{
	int		start;
	char	*var_name;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (NULL);
	var_name = ft_substr(str, start, *i - start);
	return (var_name);
}

char	*expand_single_var(char *str, int *pos, t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	(*pos)++;
	var_name = extract_var_name(str, pos);
	if (!var_name)
		return (ft_strdup("$"));
	if (ft_strcmp(var_name, "?") == 0)
	{
		free(var_name);
		return (ft_itoa(shell->exit_status));
	}
	var_value = get_env_value(var_name, shell->env);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

char	*expand_token(char *token, t_shell *shell, char quote)
{
	char	*result;

	if (!token || !ft_strchr(token, '$'))
		return (ft_strdup(token));
	if (quote == '\'')
		return (ft_strdup(token));
	result = expand_variables(token, shell, quote);
	return (result);
}
