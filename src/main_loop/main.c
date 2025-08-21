/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:27 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:06:29 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

volatile sig_atomic_t	g_sigint_recived = 0;

int	main(int ac, char **av, char **env)
{
	t_shell	shell;
	int		final_exit_code;

	(void)av;
	if (ac != 1)
	{
		write(2, "Error: ellibash does not accept arguments\n", 43);
		return (1);
	}
	init_ellibash(&shell, env);
	bash_loop(&shell);
	final_exit_code = shell.exit_status;
	shell_final_cleanup(&shell);
	return (final_exit_code);
}
