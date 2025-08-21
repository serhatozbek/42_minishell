/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:40 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:28:50 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdio.h>

void	error_exit(int status, t_shell *shell)
{
	fprintf(stderr, "ellibash: syntax error near unexpected token `newline'\n");
	shell->exit_status = status;
}

int	syntax_control(t_token *curr, t_token *tmp)
{
	if (curr->type == T_PIPE && (tmp->type == T_LESS || tmp->type == T_DLESS))
		return (2);
	if ((curr->type == T_GREAT && tmp->type == T_GREAT) || (curr->type == T_LESS
			&& tmp->type == T_LESS))
		return (2);
	if ((curr->type == T_GREAT && tmp->type == T_LESS) || (curr->type == T_LESS
			&& tmp->type == T_GREAT))
		return (2);
	else
		return (0);
}
