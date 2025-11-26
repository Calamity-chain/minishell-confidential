/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 16:59:17 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/26 19:51:41 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

int	execute_builtin(t_command *cmd, t_data *data)
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
		return (ft_exit(cmd->args, data));
	return (0);
}

static void	handle_exec_checks(t_command *cmd, t_data *data,
			char *cmd_path, struct stat *sb)
{
	if (S_ISDIR(sb->st_mode))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
		exit(126);
	}
	if (access(cmd_path, X_OK) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		exit(126);
	}
	if (execve(cmd_path, cmd->args, data->env) == -1)
		handle_execve_error(cmd->args[0]);
}

void	execute_external(t_command *cmd, t_data *data)
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
		exit(127);
	}
	handle_exec_checks(cmd, data, cmd_path, &sb);
}

void	expand_command_args(t_command *cmd, t_data *data)
{
	int	command_empty;
	int	i;

	if (!cmd || !cmd->args || !data)
		return ;
	command_empty = expand_command_name(cmd, data);
	if (command_empty)
	{
		shift_arguments_left(cmd);
		if (!cmd->args[0])
			return ;
	}
	if (!cmd->arg_quoted)
		return ;
	i = 1;
	while (cmd->args[i])
	{
		process_arg(cmd, data, i);
		i++;
	}
}
