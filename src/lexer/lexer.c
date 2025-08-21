/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:53 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:05:55 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>

static void	skip_whitespace(char **input)
{
	while (**input && is_whitespace(**input))
		(*input)++;
}

static void	link_token(t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
		*head = new_token;
	else
		(*tail)->next = new_token;
	*tail = new_token;
}

static t_token	*get_next_token(char **input)
{
	t_token	*token;

	skip_whitespace(input);
	if (!**input)
		return (NULL);
	if (is_special_char(**input))
		token = lex_special(input);
	else
		token = lex_word(input);
	return (token);
}

static t_token	*validate_syntax(t_token *head, t_shell *shell)
{
	char	*errmsg;

	errmsg = "ellibash: .: filename argument required";
	if (!head)
		return (NULL);
	if (!ft_strcmp(head->value, "."))
	{
		fprintf(stderr, "%s\n.: usage: . filename [arguments]\n", errmsg);
		shell->exit_status = 2;
		return (free_tokens(head), NULL);
	}
	if (heredo_c_syntax(head, shell))
		return (free_tokens(head), NULL);
	return (head);
}

t_token	*lexer(char *input, t_shell *shell)
{
	t_token	*head;
	t_token	*tail;
	t_token	*token;

	head = NULL;
	tail = NULL;
	while (*input)
	{
		if (tail)
			tail->has_space_after = is_whitespace(*input);
		token = get_next_token(&input);
		if (!token)
		{
			if (*input)
				return (free_tokens(head), NULL);
			break ;
		}
		link_token(&head, &tail, token);
	}
	return (validate_syntax(head, shell));
}
