/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredo.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:05:10 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:05:13 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>

static int	process_heredoc_line(char *line, t_redir *redir, int write_fd,
		t_exec_data *data)
{
	char	*expanded_line;

	if (ft_strcmp(line, redir->filename) == 0)
	{
		free(line);
		return (1);
	}
	if (redir->expand_heredoc)
	{
		expanded_line = expand_token(line, data->shell, 0);
		write(write_fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(write_fd, line, ft_strlen(line));
	write(write_fd, "\n", 1);
	free(line);
	return (0);
}

static void	read_lines_to_pipe(t_redir *redir, int write_fd, t_exec_data *data)
{
	char	*line;
	char	*error;

	error = "ellibash: warning: here-document delimited by ";
	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line && g_sigint_recived != 2)
		{
			fprintf(stderr, "%send-of-file (wanted `%s')\n", error,
				redir->filename);
			break ;
		}
		if (g_sigint_recived == 2)
			heredoc_clean_exit(data, write_fd);
		if (process_heredoc_line(line, redir, write_fd, data))
			break ;
	}
	ft_cleaner(data);
	close(write_fd);
	exit(0);
}

int	setup_heredoc_pipe(t_redir *redir, t_exec_data *data)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	t_cmd	*tmp;

	set_execution_signals();
	tmp = data->cmds;
	if (pipe(pipefd) == -1)
		return (perror("ellibash: pipe"), -1);
	pid = fork();
	if (pid == -1)
		return (perror("ellibash: fork"), -1);
	if (pid == 0)
	{
		fd_clean(tmp);
		close(pipefd[0]);
		read_lines_to_pipe(redir, pipefd[1], data);
	}
	close(pipefd[1]);
	status = wait_for_children(pid);
	set_signals();
	if (status == 130)
		return (close(pipefd[0]), -2);
	return (pipefd[0]);
}

int	heredo_c_syntax(t_token *token, t_shell *shell)
{
	t_token	*current;

	current = token;
	while (current)
	{
		if (current->type == T_DLESS || current->type == T_DGREAT
			|| current->type == T_GREAT || current->type == T_LESS)
		{
			if (!current->next || current->next->type != T_WORD)
			{
				shell->exit_status = 2;
				fprintf(stderr,
					"ellibash: syntax error near unexpected token `%s'\n",
					current->value);
				return (1);
			}
		}
		current = current->next;
	}
	return (0);
}
