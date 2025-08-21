/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:43 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:05:45 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

bool	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '\'' || c == '"'
		|| c == ' ');
}

int	just_space_check(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] != ' ' || str[i] != '\t' || str[i] != '\n')
			return (1);
		i++;
	}
	return (0);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

void	join_adjacent_words(t_token *tokens)
{
	t_token	*curr;
	char	*joined_value;
	t_token	*node_to_free;

	curr = tokens;
	while (curr && curr->next)
	{
		if (curr->type == T_WORD && curr->next->type == T_WORD
			&& curr->has_space_after == false)
		{
			joined_value = ft_strjoin(curr->value, curr->next->value);
			node_to_free = curr->next;
			free(curr->value);
			curr->value = joined_value;
			curr->has_space_after = node_to_free->has_space_after;
			curr->next = node_to_free->next;
			free(node_to_free->value);
			free(node_to_free);
		}
		else
			curr = curr->next;
	}
}
