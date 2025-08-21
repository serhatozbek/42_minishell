/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:04:49 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:27:41 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int	wait_for_children(pid_t last_pid)
{
	int		status;
	int		last_exit_status;
	pid_t	waited_pid;

	last_exit_status = 0;
	while (1)
	{
		waited_pid = wait(&status);
		if (waited_pid == -1)
			break ;
		if (waited_pid == last_pid)
		{
			if (WIFEXITED(status))
				last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_exit_status = 128 + WTERMSIG(status);
		}
	}
	return (last_exit_status);
}

void	status_check(int last_status)
{
	if (last_status == 131)
		write(STDOUT_FILENO, "exit (core dumped)", 19);
	if (last_status == 131 || last_status == 130)
		write(STDOUT_FILENO, "\n", 1);
}
