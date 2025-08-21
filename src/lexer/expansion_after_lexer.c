/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_after_lexer.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:33 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:05:35 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>

static t_token	*get_new_tail_and_set_type(t_token *new_list)
{
	t_token	*tail;

	if (!new_list)
		return (NULL);
	tail = new_list;
	while (tail->next)
	{
		tail->type = T_WORD;
		tail = tail->next;
	}
	tail->type = T_WORD;
	return (tail);
}

static void	relink_and_replace(t_token **tokens, t_token *curr, t_token *prev,
		t_token *new_list)
{
	t_token	*next_node;
	t_token	*new_tail;

	next_node = curr->next;
	if (new_list)
	{
		new_tail = get_new_tail_and_set_type(new_list);
		if (prev)
			prev->next = new_list;
		else
			*tokens = new_list;
		if (new_tail)
			new_tail->next = next_node;
	}
	else
	{
		if (prev)
			prev->next = next_node;
		else
			*tokens = next_node;
	}
	free(curr->value);
	free(curr);
}

void	expand_lexer(t_token **tokens, t_shell *shell)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*new_list;

	prev = NULL;
	curr = *tokens;
	while (curr)
	{
		if (curr->type == T_WORD && curr->quote == 0 && ft_strchr(curr->value,
				' '))
		{
			new_list = lexer(curr->value, shell);
			relink_and_replace(tokens, curr, prev, new_list);
			prev = NULL;
			curr = *tokens;
			continue ;
		}
		prev = curr;
		curr = curr->next;
	}
}
