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

static void	init_pipe_state(t_pipe_exec *st)
{
	st->in_fd = STDIN_FILENO;
	st->pipefd[0] = -1;
	st->pipefd[1] = -1;
	st->last_pid = -1;
}

static void	run_pipeline_child(t_command *cmd, t_data *data,
				t_pipe_exec *st, int has_next)
{
	setup_child_signals();
	if (st->in_fd != STDIN_FILENO)
	{
		dup2(st->in_fd, STDIN_FILENO);
		close(st->in_fd);
	}
	if (has_next)
	{
		dup2(st->pipefd[1], STDOUT_FILENO);
		close(st->pipefd[0]);
		close(st->pipefd[1]);
	}
	execute_command(cmd, data);
	exit(data->exit_status);
}

static int	fork_one_child(t_command *cmd, t_data *data,
				t_pipe_exec *st, int has_next)
{
	pid_t	pid;

	if (has_next && pipe(st->pipefd) == -1)
		return (perror("pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		run_pipeline_child(cmd, data, st, has_next);
	if (st->in_fd != STDIN_FILENO)
		close(st->in_fd);
	if (has_next)
	{
		close(st->pipefd[1]);
		st->in_fd = st->pipefd[0];
	}
	st->last_pid = pid;
	return (0);
}

static void	wait_for_pipeline_children(pid_t last_pid, t_data *data)
{
	int		status;
	pid_t	pid;

	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
			data->exit_status = get_exit_status(status);
	}
}

int	execute_pipeline(t_command *pipeline, t_data *data)
{
	t_command	*cur;
	t_pipe_exec	st;
	int			has_next;
	int			res;

	if (!pipeline)
		return (0);
	if (!pipeline->next)
		return (execute_command(pipeline, data));
	init_pipe_state(&st);
	cur = pipeline;
	while (cur)
	{
		has_next = (cur->next != NULL);
		res = fork_one_child(cur, data, &st, has_next);
		if (res != 0)
			return (1);
		cur = cur->next;
	}
	if (st.in_fd != STDIN_FILENO && st.in_fd != -1)
		close(st.in_fd);
	wait_for_pipeline_children(st.last_pid, data);
	return (data->exit_status);
}
