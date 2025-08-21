/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_ellibash.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:06:11 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:06:12 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

void	env_i_start(t_shell *shell)
{
	char	*cwd;
	char	*pwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return ;
	pwd = ft_strjoin("PWD=", cwd);
	if (!pwd)
	{
		free(cwd);
		return ;
	}
	set_env(shell, pwd);
	free(pwd);
	pwd = strjoin_three("_=", cwd, "./minishell");
	if (!pwd)
	{
		free(cwd);
		return ;
	}
	set_env(shell, pwd);
	free(pwd);
	set_env(shell, "SHLVL=1");
}

int	init_ellibash(t_shell *shell, char **env)
{
	char	*path;

	path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	shell->env = dup_env(env);
	shell->noenvpath = ft_strdup(path);
	shell->exit_status = 0;
	shell->lines = NULL;
	g_sigint_recived = 0;
	if (shell->env[0] == NULL)
		env_i_start(shell);
	set_signals();
	return (0);
}
