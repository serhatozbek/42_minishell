/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:04:55 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:27:44 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static void	child_fd_setup(t_cmd *cmd, t_exec_data *data)
{
	set_default_signals();
	if (data->in_fd != STDIN_FILENO)
	{
		dup2(data->in_fd, STDIN_FILENO);
		close(data->in_fd);
	}
	if (cmd->in_fd != STDIN_FILENO)
	{
		dup2(cmd->in_fd, STDIN_FILENO);
		close(cmd->in_fd);
	}
	if (cmd->next)
	{
		close(data->pipefd[0]);
		dup2(data->pipefd[1], STDOUT_FILENO);
		close(data->pipefd[1]);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		dup2(cmd->out_fd, STDOUT_FILENO);
		close(cmd->out_fd);
	}
}

static void	child_routine(t_cmd *cmd, t_exec_data *data)
{
	int	status;

	status = 0;
	child_fd_setup(cmd, data);
	if (!cmd->args)
	{
		ft_cleaner(data);
		exit(0);
	}
	if (cmd->args && is_builtin(cmd->args[0]))
	{
		status = exec_builtin(cmd, data->shell, 1);
		while (cmd->next)
		{
			if (cmd->next->in_fd != STDIN_FILENO)
				close(cmd->next->in_fd);
			if (cmd->next->out_fd != STDOUT_FILENO)
				close(cmd->next->out_fd);
			cmd = cmd->next;
		}
		ft_cleaner(data);
		exit(status);
	}
	else
		exec_cmd(cmd, data);
}

static void	parent_routine(t_cmd *cmd, t_exec_data *data)
{
	if (data->in_fd != STDIN_FILENO)
		close(data->in_fd);
	if (cmd->next)
	{
		close(data->pipefd[1]);
		data->in_fd = data->pipefd[0];
	}
	if (cmd->in_fd != STDIN_FILENO)
		close(cmd->in_fd);
	if (cmd->out_fd != STDOUT_FILENO)
		close(cmd->out_fd);
}

static int	execute_pipeline_loop(t_exec_data *data, t_cmd *cmd_list)
{
	pid_t	pid;
	t_cmd	*tmp;
	int		last_status;

	last_status = 0;
	tmp = cmd_list;
	set_execution_signals();
	while (tmp)
	{
		if (tmp->next && pipe(data->pipefd) == -1)
			return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
			child_routine(tmp, data);
		if (tmp->next && tmp->args)
			if (is_builtin(tmp->args[0]))
				last_status = wait_for_children(pid);
		parent_routine(tmp, data);
		tmp = tmp->next;
	}
	last_status = wait_for_children(pid);
	status_check(last_status);
	return (set_signals(), last_status);
}

int	execute_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	int			status;
	t_exec_data	data;
	t_cmd		*tmp;

	data_packet_create(&data, cmd_list, shell);
	tmp = cmd_list;
	status = 0;
	while (tmp)
	{
		status = open_redirs(tmp, &data);
		if ((status != 0 && tmp->redirs->type == T_DLESS) || (!tmp->next
				&& status != 0))
		{
			fd_clean(cmd_list);
			return (status);
		}
		tmp = tmp->next;
	}
	if (cmd_list && cmd_list->next == NULL)
	{
		status = execute_single_command(cmd_list, shell);
		if (status != -1)
			return (status);
	}
	return (execute_pipeline_loop(&data, cmd_list));
}
