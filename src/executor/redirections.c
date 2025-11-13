/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:44:44 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/14 19:44:49 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

#include "../../include/minishell.h"
#include "../../include/parser.h"

static int	check_output_perm(t_redirection *redir)
{
	int	flags;
	int	fd;

	flags = O_WRONLY | O_CREAT;
	if (redir->append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(redir->filename, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		return (1);
	}
	close(fd);
	return (0);
}

static int	check_redir_permissions(t_redirection *redir)
{
	while (redir)
	{
		if ((redir->type == REDIRECT_IN || redir->type == HEREDOC)
			&& access(redir->filename, F_OK) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(redir->filename, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (1);
		}
		if (redir->type == REDIRECT_OUT || redir->type == APPEND_OUT)
			if (check_output_perm(redir) != 0)
				return (1);
		redir = redir->next;
	}
	return (0);
}

static t_redirection	*find_last_input(t_redirection *redir)
{
	t_redirection	*last;

	last = NULL;
	while (redir)
	{
		if (redir->type == REDIRECT_IN || redir->type == HEREDOC)
			last = redir;
		redir = redir->next;
	}
	return (last);
}

static t_redirection	*find_last_output(t_redirection *redir)
{
	t_redirection	*last;

	last = NULL;
	while (redir)
	{
		if (redir->type == REDIRECT_OUT || redir->type == APPEND_OUT)
			last = redir;
		redir = redir->next;
	}
	return (last);
}

int	setup_redirections_with_data(t_command *cmd, t_data *data)
{
	t_redirection	*last_in;
	t_redirection	*last_out;

	if (!cmd)
		return (1);
	if (check_redir_permissions(cmd->redirections) != 0)
		return (1);
	last_in = find_last_input(cmd->redirections);
	if (last_in)
	{
		if (last_in->type == HEREDOC)
		{
			if (handle_heredoc(last_in->filename,
					last_in->heredoc_quoted, data) != 0)
				return (1);
		}
		else if (handle_input_redirection(last_in->filename) != 0)
			return (1);
	}
	last_out = find_last_output(cmd->redirections);
	if (last_out && handle_output_redirection(last_out->filename,
			last_out->append_mode) != 0)
		return (1);
	return (0);
}
