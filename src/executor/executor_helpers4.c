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
