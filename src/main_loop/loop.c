/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:20 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:06:22 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

static void	handle_eof(t_shell *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	shell_final_cleanup(shell);
	exit(shell->exit_status);
}

static int	process_input(t_shell *shell, char *line)
{
	t_token	*tokens;
	t_cmd	*cmds;
	int		status;

	tokens = lexer(line, shell);
	if (!tokens)
		return (0);
	expand_tokens(tokens, shell);
	expand_lexer(&tokens, shell);
	join_adjacent_words(tokens);
	remove_empty_tokens(&tokens);
	cmds = parser(tokens, shell);
	free_tokens(tokens);
	if (!cmds)
		return (0);
	status = execute_pipeline(cmds, shell);
	if (status == EXIT_SHELL)
	{
		free_cmds(cmds);
		return (EXIT_SHELL);
	}
	shell->exit_status = status;
	free_cmds(cmds);
	return (0);
}

static int	loop_newline(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->lines[i])
	{
		g_sigint_recived = 0;
		add_history(shell->lines[i]);
		if (process_input(shell, shell->lines[i]) == EXIT_SHELL)
			break ;
		if (g_sigint_recived)
			shell->exit_status = 130;
		i++;
	}
	if (shell->lines)
	{
		ft_free_split(shell->lines);
		shell->lines = NULL;
	}
	return (shell->exit_status);
}

static int	process_line(t_shell *shell, char *line)
{
	if (ft_strchr(line, '\n'))
	{
		shell->lines = ft_split(line, '\n');
		shell->exit_status = loop_newline(shell);
	}
	else
	{
		add_history(line);
		if (g_sigint_recived)
			shell->exit_status = 130;
		if (process_input(shell, line) == EXIT_SHELL)
			return (EXIT_SHELL);
	}
	return (0);
}

int	bash_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		g_sigint_recived = 0;
		line = readline("ellibash~>");
		if (!line)
			handle_eof(shell);
		if (process_line(shell, line) == EXIT_SHELL)
		{
			free(line);
			break ;
		}
		if (line)
			free(line);
	}
	return (shell->exit_status);
}
