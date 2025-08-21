/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:05 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:27:50 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <unistd.h>

int	do_single_builtin(t_cmd *cmd, t_shell *shell)
{
	int	last_exit_status;
	int	original_stdin;
	int	original_stdout;

	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (cmd->in_fd != STDIN_FILENO)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->out_fd != STDOUT_FILENO)
		dup2(cmd->out_fd, STDOUT_FILENO);
	last_exit_status = exec_builtin(cmd, shell, 0);
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
	return (last_exit_status);
}

int	execute_single_command(t_cmd *cmd, t_shell *shell)
{
	int	status;

	status = 0;
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (is_builtin(cmd->args[0]))
	{
		status = do_single_builtin(cmd, shell);
		return (status);
	}
	return (-1);
}
