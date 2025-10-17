/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:50:46 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/14 19:50:58 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

char	*ft_getenv_from_envp(char **env, const char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*build_full_path(char *dir, char *cmd)
{
	char	*full_path;
	size_t	dir_len;
	size_t	cmd_len;

	if (!dir || !cmd)
		return (NULL);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);
	ft_memcpy(full_path, dir, dir_len);
	full_path[dir_len] = '/';
	ft_memcpy(full_path + dir_len + 1, cmd, cmd_len);
	full_path[dir_len + cmd_len + 1] = '\0';
	return (full_path);
}

void	command_not_found_error(char *cmd)
{
	// Check if it looks like an unexpanded variable
	if (cmd && cmd[0] == '$')
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
}

int	get_exit_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		// Map common signals to bash-like exit codes
		int sig = WTERMSIG(status);
		if (sig == SIGINT)    // Ctrl-C
			return (130);
		else if (sig == SIGQUIT) // Ctrl-backslash
			return (131);
		return (128 + sig);
	}
	return (1);  // Default error
}

void	free_split(char **split)
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
