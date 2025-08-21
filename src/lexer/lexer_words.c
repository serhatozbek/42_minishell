/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:48 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:28:16 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdlib.h>

t_token	*new_token(t_token_type type, char *value, char quote)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->quote = quote;
	token->has_space_after = false;
	token->next = NULL;
	return (token);
}

static int	get_scanned_len(char *input)
{
	int	i;

	i = 0;
	while (input[i] && !is_whitespace(input[i]) && !is_special_char(input[i]))
	{
		if (input[i] == '\\' && input[i + 1])
			i++;
		i++;
	}
	return (i);
}

static char	*build_clean_word(char *input, int scanned_len)
{
	char	*word;
	int		i;
	int		j;

	word = malloc(sizeof(char) * (scanned_len + 1));
	if (!word)
		return (NULL);
	i = 0;
	j = 0;
	while (i < scanned_len)
	{
		if (input[i] == '\\' && input[i + 1])
			i++;
		word[j++] = input[i++];
	}
	word[j] = '\0';
	return (word);
}

t_token	*lex_word(char **input)
{
	char	*word_value;
	int		scanned_len;

	scanned_len = get_scanned_len(*input);
	word_value = build_clean_word(*input, scanned_len);
	if (!word_value)
		return (NULL);
	*input += scanned_len;
	return (new_token(T_WORD, word_value, '\0'));
}
