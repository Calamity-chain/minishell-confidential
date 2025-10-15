/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:44:57 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/14 19:45:00 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

static void	close_pipe(int pipefd[2])
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
	pipefd[0] = -1;
	pipefd[1] = -1;
}

static void	setup_next_pipe(t_command *cmd, t_pipe_state *state)
{
	if (cmd && cmd->next)
	{
		if (pipe(state->pipefd) == -1)
			perror("pipe");
	}
}

int	setup_pipe_redirections(t_command *cmd, t_command *pipeline, t_pipe_state *state)
{
	(void)pipeline;
	
	if (state->prev_pipe_read != -1)
	{
		if (dup2(state->prev_pipe_read, STDIN_FILENO) == -1)
			return (perror("dup2"), -1);
		close(state->prev_pipe_read);
		state->prev_pipe_read = -1;
	}
	
	setup_next_pipe(cmd, state);
	
	if (state->pipefd[1] != -1)
	{
		if (dup2(state->pipefd[1], STDOUT_FILENO) == -1)
			return (perror("dup2"), -1);
		close(state->pipefd[1]);
		state->pipefd[1] = -1;
	}
	
	state->prev_pipe_read = state->pipefd[0];
	state->pipefd[0] = -1;
	
	return (0);
}

void	cleanup_pipe_redirections(t_command *cmd, t_pipe_state *state)
{
	(void)cmd;
	if (state->prev_pipe_read != -1)
	{
		close(state->prev_pipe_read);
		state->prev_pipe_read = -1;
	}
	close_pipe(state->pipefd);
}

char	*expand_heredoc_line(char *line, t_data *data)
{
	if (!line || !data)
		return (ft_strdup(line));
	
	// Use the new expansion function from expansion.c
	return (expand_env_variable_in_string(data, line));
}
