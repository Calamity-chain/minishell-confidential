/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:43:55 by ltoscani          #+#    #+#             */
/*   Updated: 2025/11/26 21:05:08 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

static int	setup_stdio_and_redirs(t_command *cmd, t_data *data,
				int *in_backup, int *out_backup)
{
	*in_backup = dup(STDIN_FILENO);
	*out_backup = dup(STDOUT_FILENO);
	if (*in_backup == -1 || *out_backup == -1)
	{
		perror("dup");
		if (*in_backup != -1)
			close(*in_backup);
		if (*out_backup != -1)
			close(*out_backup);
		return (1);
	}
	if (setup_redirections_with_data(cmd, data) != 0)
	{
		restore_fds(*in_backup, *out_backup);
		data->exit_status = 1;
		return (1);
	}
	return (0);
}

int	launch_child(t_command *cmd, t_data *data,
				int in_backup, int out_backup)
{
	pid_t	pid;
	int		status;
	int		exit_status;

	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execute_external(cmd, data);
	}
	if (pid < 0)
	{
		perror("fork");
		exit_status = 1;
	}
	else
	{
		waitpid(pid, &status, 0);
		exit_status = get_exit_status(status);
	}
	restore_fds(in_backup, out_backup);
	data->exit_status = exit_status;
	return (exit_status);
}

static char	*find_in_path_dirs(char **dirs, char *cmd)
{
	int		i;
	char	*full;

	i = 0;
	while (dirs[i])
	{
		full = build_full_path(dirs[i], cmd);
		if (full && access(full, X_OK) == 0)
		{
			free_split(dirs);
			return (full);
		}
		free(full);
		i++;
	}
	free_split(dirs);
	return (NULL);
}

char	*find_command_path(char *cmd, char **env)
{
	char		*path;
	char		**dirs;
	struct stat	status_buffer;

	if (!cmd || !env || !*cmd)
		return (NULL);
	if (cmd[0] == '/' || ft_strchr(cmd, '/')
		|| ft_strncmp(cmd, ".", 2) == 0
		|| ft_strncmp(cmd, "..", 3) == 0)
	{
		if (stat(cmd, &status_buffer) == -1)
			return (NULL);
		return (ft_strdup(cmd));
	}
	path = ft_getenv_from_envp(env, "PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	return (find_in_path_dirs(dirs, cmd));
}

int	execute_command(t_command *cmd, t_data *data)
{
	int	in_backup;
	int	out_backup;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	expand_command_args(cmd, data);
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
	{
		data->exit_status = 0;
		return (0);
	}
	if (setup_stdio_and_redirs(cmd, data, &in_backup, &out_backup) != 0)
	{
		data->exit_status = 1;
		return (1);
	}
	return (run_command_after_redirs(cmd, data, in_backup, out_backup));
}