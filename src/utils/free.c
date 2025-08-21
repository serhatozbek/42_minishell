/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:07:30 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:29:06 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include "../../inc/libft/libft.h"
#include <stdlib.h>
#include <readline/readline.h>

static void	free_redirs(t_redir *list)
{
	t_redir	*tmp;

	while (list)
	{
		tmp = list->next;
		if (list->filename)
			free(list->filename);
		free(list);
		list = tmp;
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		j;

	while (cmds)
	{
		tmp = cmds->next;
		if (cmds->args)
		{
			j = 0;
			while (cmds->args[j])
			{
				free(cmds->args[j]);
				j++;
			}
			free(cmds->args);
		}
		if (cmds->redirs)
			free_redirs(cmds->redirs);
		free(cmds);
		cmds = tmp;
	}
}

void	shell_final_cleanup(t_shell *shell)
{
	if (shell)
	{
		if (shell->lines)
		{
			ft_free_split(shell->lines);
			shell->lines = NULL;
		}
		if (shell->noenvpath)
			free(shell->noenvpath);
		if (shell->env)
		{
			ft_free_split(shell->env);
			shell->env = NULL;
		}
		rl_clear_history();
	}
}

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}
