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
	char		*path;
	char		*dir;
	char		*full_path;
	char		**path_dirs;
	int			i;
	struct stat	sb;

	if (!cmd || !env || cmd[0] == '\0')
		return (NULL);
	
	// Handle absolute paths, relative paths with /, ., and ..
	if (cmd[0] == '/' || ft_strchr(cmd, '/') || 
		ft_strncmp(cmd, ".", 2) == 0 || ft_strncmp(cmd, "..", 3) == 0)
	{
		if (stat(cmd, &sb) == -1)
		{
			// File doesn't exist at all
			return (NULL);
		}
		// File/directory exists - return it for further checking
		return (ft_strdup(cmd));
	}
	
	// Search in PATH (only for commands without /)
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
	char		*cmd_path;
	struct stat	sb;

	cmd_path = find_command_path(cmd->args[0], data->env);
	
	if (!cmd_path)
	{
		command_not_found_error(cmd->args[0]);
		exit(127);
	}
	
	if (stat(cmd_path, &sb) == -1)
	{
		command_not_found_error(cmd->args[0]);
		free(cmd_path);
		exit(127);
	}
	
	// CRITICAL FIX: Handle directory case
	if (S_ISDIR(sb.st_mode))
	{
		// If command came from environment variable expansion, treat as command not found
		if (cmd->from_env_var)
		{
			command_not_found_error(cmd->args[0]);
			free(cmd_path);
			exit(127);
		}
		else
		{
			// Explicit directory path -> "Is a directory"
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			free(cmd_path);
			exit(126);
		}
	}
	
	// Check if executable (for regular files)
	if (access(cmd_path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		free(cmd_path);
		exit(126);
	}
	
	// Everything is good, execute
	if (execve(cmd_path, cmd->args, data->env) == -1)
	{
		perror(cmd->args[0]);
		free(cmd_path);
		exit(126);
	}
}
/**
 * @brief Remove only the outer delimiting quotes from a string
 */
static char	*remove_outer_quotes(char *str)
{
	char	*result;
	size_t	len;
	char	first_char;

	if (!str || ft_strlen(str) < 2)
		return (ft_strdup(str));
	
	first_char = str[0];
	len = ft_strlen(str);
	
	// Only remove if the first and last characters are matching quotes
	if ((first_char == '\'' || first_char == '"') && str[len - 1] == first_char)
	{
		// Remove both outer quotes
		result = ft_substr(str, 1, len - 2);
		//free(str); // DANGEROUS - caller might not expect this
	}
	else
	{
		result = str;
	}
	
	return (result);
}

/**
 * @brief Shift arguments left when command name is empty
 */
static void	shift_arguments_left(t_command *cmd)
{
	int	i;

	if (!cmd->args[1])
	{
		// No other arguments, command is completely empty
		free(cmd->args[0]);
		cmd->args[0] = NULL;
		return;
	}
	
	// Shift arguments left
	free(cmd->args[0]);
	i = 1;
	while (cmd->args[i])
	{
		cmd->args[i - 1] = cmd->args[i];
		i++;
	}
	cmd->args[i - 1] = NULL;
}
/**
 * @brief Expand environment variables in command name (first argument)
 * Returns 1 if command became empty after expansion, 0 otherwise
 */
static int	expand_command_name(t_command *cmd, t_data *data)
{
	char	*expanded;

	if (!cmd || !cmd->args || !cmd->args[0] || !data)
		return (0);
	
	// Check if command starts with $ (environment variable)
	if (cmd->args[0][0] == '$')
		cmd->from_env_var = 1;
	
	// Expand environment variables in command name
	expanded = expand_env_variable_in_string(data, cmd->args[0]);
	if (expanded)
	{
		free(cmd->args[0]);
		cmd->args[0] = expanded;
	}	
	// Handle tilde expansion in command name
	if (cmd->args[0][0] == '~' && (cmd->args[0][1] == '/' || cmd->args[0][1] == '\0'))
	{
		expanded = expand_tilda(data, cmd->args[0]);
		if (expanded)
		{
			free(cmd->args[0]);
			cmd->args[0] = expanded;
		}
	}
	
	// Check if command name became empty after expansion
	if (cmd->args[0][0] == '\0')
		return (1);
	
	return (0);
}

void	expand_command_args(t_command *cmd, t_data *data)
{
	int		i;
	char	*expanded;
	char	*arg;
	int		command_empty;

	if (!cmd || !cmd->args || !data || !cmd->arg_quoted)
		return;
	
	// First expand the command name itself
	command_empty = expand_command_name(cmd, data);
	
	// If command name became empty after expansion, shift arguments
	if (command_empty)
	{
		shift_arguments_left(cmd);
		// If there's no new command after shifting, return
		if (!cmd->args[0])
			return;
	}
	
	// Then expand the rest of the arguments
	i = 1;
	while (cmd->args[i])
	{
		arg = cmd->args[i];
		
		// Handle environment variable expansion (respecting quotes)
		if (cmd->arg_quoted[i] != Q_SQUOTE)
		{
			expanded = expand_env_variable_in_string(data, arg);
			if (expanded)
			{
				free(cmd->args[i]);
				cmd->args[i] = expanded;
				arg = expanded;
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
				arg = expanded;
			}
		}
		
		// Remove outer quotes LAST
		cmd->args[i] = remove_outer_quotes(arg);
		i++;
	}
}

int	execute_command(t_command *cmd, t_data *data)
{
	int		original_stdin;
	int		original_stdout;
	int		exit_status;
	int		status;
	pid_t	pid;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	
	// Expand environment variables in command arguments
	expand_command_args(cmd, data);
	
	// CRITICAL FIX: Check for empty command after expansion
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
	{
		// Empty command - treat as success (like bash)
		return (0);
	}
	
	original_stdin = dup(STDIN_FILENO);
	original_stdout = dup(STDOUT_FILENO);
	if (original_stdin == -1 || original_stdout == -1)
		return (perror("dup"), 1);
	
	// Use the setup_redirections_with_data from redirections.c
	if (setup_redirections_with_data(cmd, data) != 0)
	{
		restore_fds(original_stdin, original_stdout);
		data->exit_status = 1; 
		return (1);  // Return error code 1 for redirection failures
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
	int			stdin_backup;
	int			stdout_backup;

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
	
	// Save original file descriptors
	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
		return (perror("dup"), 1);
	
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
			close(stdin_backup);
			close(stdout_backup);
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
			close(stdin_backup);
			close(stdout_backup);
			return (1);
		}
		
		if (pids[i] == 0)
		{
			// Child process
			setup_child_signals();
			
			// Set up pipe redirections
			if (i > 0) // Not first command - connect to previous pipe
			{
				dup2(pipes[i-1][0], STDIN_FILENO);
			}
			
			if (i < num_commands - 1) // Not last command - connect to next pipe  
			{
				dup2(pipes[i][1], STDOUT_FILENO);
			}
			
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
	
	// Restore original file descriptors
	restore_fds(stdin_backup, stdout_backup);
	
	return (data->exit_status);
}
