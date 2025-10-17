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

static int	handle_input_redirection(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_output_redirection(char *filename, int append_mode)
{
	int	fd;
	int	flags;

	if (!filename)
		return (-1);
	flags = O_WRONLY | O_CREAT;
	if (append_mode)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_heredoc(char *delimiter, int quoted, t_data *data)
{
	int		pipefd[2];
	char	*line;
	char	*expanded_line;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		
		if (!quoted)
		{
			expanded_line = expand_heredoc_line(line, data);
			free(line);
			line = expanded_line;
		}
		
		if (line)
		{
			write(pipefd[1], line, ft_strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}
	}
	
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (close(pipefd[0]), -1);
	close(pipefd[0]);
	return (0);
}

int	setup_redirections_with_data(t_command *cmd, t_data *data)
{
	if (cmd->input_file)
	{
		if (handle_input_redirection(cmd->input_file) != 0)
			return (-1);
	}
	if (cmd->heredoc_delim)
	{
		if (handle_heredoc(cmd->heredoc_delim, cmd->heredoc_quoted, data) != 0)
			return (-1);
	}
	if (cmd->output_file)
	{
		if (handle_output_redirection(cmd->output_file, cmd->append_mode) != 0)
			return (-1);
	}
	return (0);
}

void	restore_fds(int stdin_fd, int stdout_fd)
{
	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}
