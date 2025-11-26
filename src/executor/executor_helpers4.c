/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helpers4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 22:08:45 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/24 22:08:47 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/parser.h"

void	handle_execve_error(char *cmd)
{
	if (errno == EISDIR)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	if (errno == ENOENT)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	perror(cmd);
	exit(126);
}

int	run_command_after_redirs(t_command *cmd, t_data *data,
			int in_backup, int out_backup)
{
	int	exit_status;

	if (is_builtin(cmd->args[0]))
	{
		exit_status = execute_builtin(cmd, data);
		restore_fds(in_backup, out_backup);
		data->exit_status = exit_status;
		return (exit_status);
	}
	exit_status = launch_child(cmd, data, in_backup, out_backup);
	data->exit_status = exit_status;
	return (exit_status);
}

char	*expand_non_quoted_arg(t_command *cmd, t_data *data, int i)
{
	char	*arg;
	char	*tmp;

	arg = cmd->args[i];
	tmp = expand_env_variable_in_string(data, arg);
	if (tmp)
	{
		free(cmd->args[i]);
		cmd->args[i] = tmp;
		arg = tmp;
	}
	if (arg[0] == '~' && (arg[1] == '/' || !arg[1]))
	{
		tmp = expand_tilda(data, arg);
		if (tmp)
		{
			free(cmd->args[i]);
			cmd->args[i] = tmp;
			arg = tmp;
		}
	}
	return (cmd->args[i]);
}

void	process_arg(t_command *cmd, t_data *data, int i)
{
	char	*arg;
	char	*clean;

	arg = cmd->args[i];
	if (cmd->arg_quoted[i] != Q_SQUOTE)
		arg = expand_non_quoted_arg(cmd, data, i);
	clean = remove_outer_quotes(arg);
	if (!clean)
		return ;
	free(cmd->args[i]);
	cmd->args[i] = clean;
}
