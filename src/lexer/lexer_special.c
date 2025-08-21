/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_special.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:38 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:05:40 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

char	*extract_quoted(char **input, char quote)
{
	char	*start;
	int		len;
	char	*result;

	start = *input + 1;
	len = 0;
	while (start[len] && start[len] != quote)
		len++;
	if (start[len] != quote)
		return (NULL);
	result = ft_substr(start, 0, len);
	if (!result)
		return (NULL);
	*input = start + len + 1;
	return (result);
}

static t_token	*handle_double_char_tokens(char **input)
{
	if (**input == '<' && *(*input + 1) == '<')
	{
		*input += 2;
		return (new_token(T_DLESS, ft_strdup("<<"), '\0'));
	}
	else if (**input == '>' && *(*input + 1) == '>')
	{
		*input += 2;
		return (new_token(T_DGREAT, ft_strdup(">>"), '\0'));
	}
	return (NULL);
}

static t_token	*handle_single_char_tokens(char **input)
{
	t_token_type	type;

	if (**input == '|')
		type = T_PIPE;
	else if (**input == '<')
		type = T_LESS;
	else if (**input == '>')
		type = T_GREAT;
	else
		return (NULL);
	(*input)++;
	if (type == T_PIPE)
		return (new_token(type, ft_strdup("|"), '\0'));
	else if (type == T_LESS)
		return (new_token(type, ft_strdup("<"), '\0'));
	else
		return (new_token(type, ft_strdup(">"), '\0'));
}

static t_token	*handle_quote_tokens(char **input)
{
	char	quote_char;
	char	*value;

	if (**input == '\'' || **input == '"')
	{
		quote_char = **input;
		value = extract_quoted(input, quote_char);
		if (!value)
			return (NULL);
		return (new_token(T_WORD, value, quote_char));
	}
	return (NULL);
}

t_token	*lex_special(char **input)
{
	t_token	*token;

	token = handle_double_char_tokens(input);
	if (token)
		return (token);
	token = handle_single_char_tokens(input);
	if (token)
		return (token);
	token = handle_quote_tokens(input);
	return (token);
}
