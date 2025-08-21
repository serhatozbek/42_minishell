/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_token.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:59 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:06:00 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

static t_token	*handle_token_deletion(t_token **head, t_token *prev,
		t_token *curr)
{
	t_token	*next_node;

	next_node = curr->next;
	if (prev == NULL)
		*head = next_node;
	else
		prev->next = next_node;
	free(curr->value);
	free(curr);
	return (next_node);
}

void	remove_empty_tokens(t_token **head)
{
	t_token	*curr;
	t_token	*prev;

	if (!head || !*head)
		return ;
	curr = *head;
	prev = NULL;
	while (curr)
	{
		if (curr->type == T_WORD && curr->value[0] == '\0' && curr->quote == 0)
			curr = handle_token_deletion(head, prev, curr);
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}
