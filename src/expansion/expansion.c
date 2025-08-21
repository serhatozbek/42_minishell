/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:26 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:28:01 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdlib.h>

static char	*append_regular_text(char *result, char *str, int *i)
{
	char	*temp_substr;
	char	*temp_result;
	int		start;

	start = *i;
	while (str[*i] && str[*i] != '$')
		(*i)++;
	if (*i > start)
	{
		temp_substr = ft_substr(str, start, *i - start);
		temp_result = result;
		result = ft_strjoin(temp_result, temp_substr);
		free(temp_substr);
		free(temp_result);
	}
	return (result);
}

static char	*append_expanded_variable(char *result, char *str, int *i,
		t_shell *shell)
{
	char	*var_expansion;
	char	*temp_result;

	var_expansion = expand_single_var(str, i, shell);
	temp_result = result;
	result = ft_strjoin(temp_result, var_expansion);
	free(var_expansion);
	free(temp_result);
	return (result);
}

char	*expand_variables(char *str, t_shell *shell, char quote)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && quote != '\'')
			result = append_expanded_variable(result, str, &i, shell);
		else if (str[i] == '$')
		{
			result = ft_strjoin_free(result, "$");
			i++;
		}
		else
			result = append_regular_text(result, str, &i);
	}
	return (result);
}

static void	expand_token_value(t_token *token, t_shell *shell)
{
	char	*expanded_value;

	if (token->type == T_WORD && token->value && ft_strchr(token->value, '$'))
	{
		if (token->quote != '\'')
		{
			expanded_value = expand_variables(token->value, shell,
					token->quote);
			free(token->value);
			token->value = expanded_value;
		}
	}
}

int	expand_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*curr;
	t_token	*prev;

	curr = tokens;
	prev = NULL;
	while (curr)
	{
		if (prev && prev->type == T_DLESS)
		{
			prev = curr;
			curr = curr->next;
			continue ;
		}
		expand_token_value(curr, shell);
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
