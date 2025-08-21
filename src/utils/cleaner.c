/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:07:24 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:29:00 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>

void	fd_clean(t_cmd *tmp)
{
	while (tmp)
	{
		if (tmp->in_fd != STDIN_FILENO)
			close(tmp->in_fd);
		if (tmp->out_fd != STDOUT_FILENO)
			close(tmp->out_fd);
		tmp = tmp->next;
	}
}

void	ft_cleaner(t_exec_data *data)
{
	rl_clear_history();
	fd_clean(data->cmds);
	free_cmds(data->cmds);
	shell_final_cleanup(data->shell);
}

void	heredoc_clean_exit(t_exec_data *data, int write_fd)
{
	g_sigint_recived = 0;
	rl_clear_history();
	ft_cleaner(data);
	close(write_fd);
	exit(130);
}
