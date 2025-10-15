/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:43:55 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/14 19:50:40 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

static int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

static int	execute_builtin(t_command *cmd, t_data *data)
{
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(cmd->args));
	if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, data));
	if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd());
	if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (ft_export(cmd->args, data));
	if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (ft_unset(cmd->args, data));
	if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (ft_env(cmd->args, data));
	if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		return (ft_exit(cmd->args));
	return (0);
}

static char	*find_command_path(char *cmd, char **env)
{
	char	*path;
	char	*dir;
	char	*full_path;
	char	**path_dirs;
	int		i;

	if (!cmd || !env || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/') || ft_strncmp(cmd, ".", 2) == 0 || ft_strncmp(cmd, "..", 3) == 0)
		return (ft_strdup(cmd));
	path = ft_getenv_from_envp(env, "PATH");
	if (!path)
		return (NULL);
	path_dirs = ft_split(path, ':');
	if (!path_dirs)
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		dir = path_dirs[i];
		full_path = build_full_path(dir, cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			free_split(path_dirs);
			return (full_path);
		}
		free(full_path);
	}
	free_split(path_dirs);
	return (NULL);
}

static void	execute_external(t_command *cmd, t_data *data)
{
	char	*cmd_path;

	cmd_path = find_command_path(cmd->args[0], data->env);
	if (!cmd_path)
	{
		command_not_found_error(cmd->args[0]);
		exit(127); // Exit instead of return for child processes
	}
	
	if (execve(cmd_path, cmd->args, data->env) == -1)
	{
		perror(cmd->args[0]);
		free(cmd_path);
		exit(126); // Exit instead of return
	}
	
	// Should not reach here
	free(cmd_path);
	exit(1);
}

void	expand_command_args(t_command *cmd, t_data *data)
{
	int		i;
	char	*expanded;
	char	*arg;

	if (!cmd || !cmd->args || !data || !cmd->arg_quoted)
		return ;
	
	i = 0;
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		
		// Use the actual quote information from parsing
		if (cmd->arg_quoted[i] != Q_SQUOTE) // Only expand if not single-quoted
		{
			if (ft_strchr(arg, '$') || ft_strchr(arg, '\'') || ft_strchr(arg, '"'))
			{
				expanded = expand_env_variable_in_string(data, arg);
				if (expanded)
				{
					free(cmd->args[i]);
					cmd->args[i] = expanded;
				}
			}
		}
		
		// Handle tilde expansion (only if not single-quoted)
		if (cmd->arg_quoted[i] != Q_SQUOTE && 
			arg[0] == '~' && (arg[1] == '/' || arg[1] == '\0'))
		{
			expanded = expand_tilda(data, arg);
			if (expanded)
			{
				free(cmd->args[i]);
				cmd->args[i] = expanded;
			}
		}
		i++;
	}
}

static int	setup_redirections_with_data(t_command *cmd, t_data *data)
{
	int	fd;

	if (cmd->input_file)
	{
		fd = open(cmd->input_file, O_RDONLY);
		if (fd == -1)
			return (perror(cmd->input_file), -1);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (close(fd), perror("dup2"), -1);
		close(fd);
	}
	if (cmd->heredoc_delim)
	{
		if (handle_heredoc(cmd->heredoc_delim, cmd->heredoc_quoted, data) != 0)
			return (-1);
	}
	if (cmd->output_file)
	{
		fd = open(cmd->output_file, O_WRONLY | O_CREAT | (cmd->append_mode ? O_APPEND : O_TRUNC), 0644);
		if (fd == -1)
			return (perror(cmd->output_file), -1);
		if (dup2(fd, STDOUT_FILENO) == -1)
			return (close(fd), perror("dup2"), -1);
		close(fd);
	}
	return (0);
}

int	execute_command(t_command *cmd, t_data *data)
{
	int	original_stdin;
	int	original_stdout;
	int	exit_status;
	int	status;
	pid_t	pid;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
	// Expand environment variables in command arguments
	expand_command_args(cmd, data);
	
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdin == -1 || original_stdout == -1)
		return (perror("dup"), 1);
	
	if (setup_redirections_with_data(cmd, data) != 0)
	{
		restore_fds(original_stdin, original_stdout);
		return (1);
	}
	
	// Handle builtin commands (no fork needed)
	if (is_builtin(cmd->args[0]))
	{
		exit_status = execute_builtin(cmd, data);
		restore_fds(original_stdin, original_stdout);
		data->exit_status = exit_status;
		return (exit_status);
	}
	
	// Handle external commands (fork required)
	pid = fork();
	if (pid == 0)
	{
		// Child process
		setup_child_signals();
		execute_external(cmd, data);
		// If we reach here, execute_external failed
		exit(1);
	}
	else if (pid > 0)
	{
		// Parent process
		waitpid(pid, &status, 0);
		exit_status = get_exit_status(status);
	}
	else
	{
		perror("fork");
		exit_status = 1;
	}
	
	restore_fds(original_stdin, original_stdout);
	data->exit_status = exit_status;
	return (exit_status);
}

int	execute_pipeline(t_command *pipeline, t_data *data)
{
	t_command	*current;
	pid_t		*pids;
	int			num_commands;
	int			i;
	int			**pipes;
	int			status;

	if (!pipeline)
		return (0);
	
	// Count commands in pipeline
	num_commands = 0;
	current = pipeline;
	while (current)
	{
		num_commands++;
		current = current->next;
	}
	
	if (num_commands == 1)
		return (execute_command(pipeline, data));
	
	// Allocate PID array and pipes
	pids = malloc(sizeof(pid_t) * num_commands);
	pipes = malloc(sizeof(int *) * (num_commands - 1));
	
	for (i = 0; i < num_commands - 1; i++)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			for (int j = 0; j < i; j++)
				free(pipes[j]);
			free(pipes);
			free(pids);
			return (1);
		}
	}
	
	// Execute all commands
	current = pipeline;
	i = 0;
	while (current)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			for (int j = 0; j < num_commands - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				free(pipes[j]);
			}
			free(pipes);
			free(pids);
			return (1);
		}
		
		if (pids[i] == 0)
		{
			// Child process
			setup_child_signals();
			
			// Set up pipe redirections
			if (i > 0) // Not first command - connect to previous pipe
				dup2(pipes[i-1][0], STDIN_FILENO);
			
			if (i < num_commands - 1) // Not last command - connect to next pipe  
				dup2(pipes[i][1], STDOUT_FILENO);
			
			// Close all pipe ends in child
			for (int j = 0; j < num_commands - 1; j++)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
			}	
			// Execute command and exit
			execute_command(current, data);
			exit(data->exit_status);
		}
		
		current = current->next;
		i++;
	}
	
	// Parent process - close all pipe ends
	for (i = 0; i < num_commands - 1; i++)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
	}
	free(pipes);
	
	// Wait for all children
	for (i = 0; i < num_commands; i++)
	{
		waitpid(pids[i], &status, 0);
		if (i == num_commands - 1)
			data->exit_status = get_exit_status(status);
	}
	
	free(pids);
	return (data->exit_status);
}
