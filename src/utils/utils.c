/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sozbek <sozbek@student.kocaeli.42.tr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 04:07:35 by sozbek            #+#    #+#             */
/*   Updated: 2025/08/21 04:07:37 by sozbek           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft/libft.h"
#include <stdlib.h>
#include <unistd.h>

char	*strjoin_three(const char *s1, const char *s2, const char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1)
		return (0);
	result = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!result)
		return (0);
	while (s1[i])
		result[j++] = s1[i++];
	i = 0;
	if (!s2)
		return (result);
	while (s2[i])
		result[j++] = s2[i++];
	result[j] = '\0';
	free(s1);
	return (result);
}

char	**dup_env(char **env)
{
	int		i;
	char	**dup_env;

	i = 0;
	while (env[i])
		i++;
	dup_env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!dup_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		dup_env[i] = ft_strdup(env[i]);
		if (!dup_env[i])
		{
			while (i--)
				free(dup_env[i]);
			free(dup_env);
			return (NULL);
		}
		i++;
	}
	dup_env[i] = NULL;
	return (dup_env);
}

void	data_packet_create(t_exec_data *data, t_cmd *cmd, t_shell *shell)
{
	data->in_fd = STDIN_FILENO;
	data->shell = shell;
	data->cmds = cmd;
}
