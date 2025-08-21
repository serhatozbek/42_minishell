/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 17:53:39 by ocubukcu          #+#    #+#             */
/*   Updated: 2025/08/21 04:07:10 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>

static int	process_token(t_token **curr, t_cmd *cmd)
{
	int	status;

	if (((*curr)->type == T_LESS || (*curr)->type == T_GREAT
			|| (*curr)->type == T_DLESS || (*curr)->type == T_DGREAT))
	{
		status = parse_redir(curr, cmd);
		if (status == 127)
			return (127);
		if (status == 2)
			return (2);
	}
	else if ((*curr)->type == T_WORD)
	{
		if (add_args(cmd, (*curr)->value) != 0)
			return (1);
		*curr = (*curr)->next;
	}
	return (0);
}

static t_cmd	*parse_cmd(t_token **tokens, t_shell *shell)
{
	t_cmd	*cmd;
	t_token	*curr;
	int		status;

	cmd = new_cmd();
	if (!cmd)
		return (NULL);
	curr = *tokens;
	while (curr && curr->type != T_PIPE)
	{
		status = process_token(&curr, cmd);
		if (status == 127 || status == 2)
		{
			error_exit(status, shell);
			return (free_cmds(cmd), NULL);
		}
	}
	*tokens = curr;
	return (cmd);
}

static void	link_new_cmd(t_cmd **head, t_cmd **prev, t_cmd *new_cmd)
{
	if (!*head)
		*head = new_cmd;
	else
	{
		(*prev)->next = new_cmd;
		new_cmd->type = T_PIPE;
	}
	*prev = new_cmd;
}

static bool	handle_pipe_token(t_token **tokens)
{
	if (*tokens && (*tokens)->type == T_PIPE)
	{
		*tokens = (*tokens)->next;
		if (!*tokens || (*tokens)->type == T_PIPE)
		{
			fprintf(stderr,
				"ellibash: error near unexpected token `|'\n");
			return (false);
		}
	}
	return (true);
}

t_cmd	*parser(t_token *tokens, t_shell *shell)
{
	t_cmd	*head;
	t_cmd	*cmd;
	t_cmd	*prev;

	head = NULL;
	prev = NULL;
	if (tokens && tokens->type == T_PIPE)
	{
		fprintf(stderr, "ellibash: syntax error near unexpected token `|'\n");
		shell->exit_status = 2;
		return (NULL);
	}
	while (tokens)
	{
		cmd = parse_cmd(&tokens, shell);
		if (!cmd)
			return (free_cmds(head), NULL);
		link_new_cmd(&head, &prev, cmd);
		if (!handle_pipe_token(&tokens))
		{
			shell->exit_status = 2;
			return (free_cmds(head), NULL);
		}
	}
	return (head);
}
