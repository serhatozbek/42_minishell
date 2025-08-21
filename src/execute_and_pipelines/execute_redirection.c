/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:00 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:05:02 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

static int	handle_input_redir(t_cmd *cmd, t_redir *redir, t_exec_data *data)
{
	int	fd;

	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	if (redir->type == T_LESS)
		fd = open(redir->filename, O_RDONLY);
	else
		fd = setup_heredoc_pipe(redir, data);
	if (fd == -1)
	{
		fprintf(stderr, "ellibash: %s: No such file or directory\n",
			redir->filename);
		return (1);
	}
	if (fd == -2)
		return (130);
	cmd->in_fd = fd;
	return (0);
}

static int	handle_output_redir(t_cmd *cmd, t_redir *redir)
{
	int	fd;

	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
	if (redir->type == T_GREAT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror(redir->filename), 1);
	cmd->out_fd = fd;
	return (0);
}

int	open_redirs(t_cmd *cmd, t_exec_data *data)
{
	t_redir	*curr;
	int		a;

	a = 0;
	curr = cmd->redirs;
	while (curr)
	{
		if (curr->type == T_LESS || curr->type == T_DLESS)
		{
			a = handle_input_redir(cmd, curr, data);
			if (a != 0)
				return (a);
		}
		else if (curr->type == T_GREAT || curr->type == T_DGREAT)
		{
			if (handle_output_redir(cmd, curr) != 0)
				return (1);
		}
		curr = curr->next;
	}
	return (0);
}
