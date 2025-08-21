/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:49 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:28:54 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	cmd->type = T_WORD;
	return (cmd);
}

static char	**reallocate_and_add_arg(char **old_args, char *new_arg_str,
		int count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	i = -1;
	while (++i < count)
		new_args[i] = old_args[i];
	new_args[i] = ft_strdup(new_arg_str);
	if (!new_args[i])
	{
		free(new_args);
		return (NULL);
	}
	new_args[i + 1] = NULL;
	if (old_args)
		free(old_args);
	return (new_args);
}

int	add_args(t_cmd *cmd, char *arg)
{
	int		arg_count;
	char	**new_args;

	arg_count = 0;
	if (cmd->args)
		while (cmd->args[arg_count])
			arg_count++;
	new_args = reallocate_and_add_arg(cmd->args, arg, arg_count);
	if (!new_args)
		return (1);
	cmd->args = new_args;
	return (0);
}

static void	add_redir_to_list(t_redir **list, t_redir *new_redir)
{
	t_redir	*curr;

	if (!*list)
	{
		*list = new_redir;
		return ;
	}
	curr = *list;
	while (curr->next)
		curr = curr->next;
	curr->next = new_redir;
}

int	parse_redir(t_token **curr, t_cmd *cmd)
{
	t_redir	*redir;
	t_token	*tmp;

	redir = malloc(sizeof(t_redir));
	redir->type = (*curr)->type;
	redir->next = NULL;
	redir->expand_heredoc = true;
	tmp = *curr;
	if (!(*curr)->next)
		return (2);
	(*curr) = (*curr)->next;
	if (!(*curr) || (*curr)->type != T_WORD)
	{
		free(redir);
		if (syntax_control(*curr, tmp))
			return (2);
		return (127);
	}
	redir->filename = ft_strdup((*curr)->value);
	if (redir->type == T_DLESS && (*curr)->quote != 0)
		redir->expand_heredoc = false;
	add_redir_to_list(&cmd->redirs, redir);
	(*curr) = (*curr)->next;
	return (0);
}
