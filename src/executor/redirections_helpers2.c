#include "../../include/minishell.h"
#include "../../include/parser.h"

static void	process_heredoc_line(int fd, char **line,
			int quoted, t_data *data)
{
	char	*expanded;

	if (!*line)
		return ;
	if (!quoted)
	{
		expanded = expand_heredoc_line(*line, data);
		free(*line);
		*line = expanded;
	}
	if (!*line)
		return ;
	write(fd, *line, ft_strlen(*line));
	write(fd, "\n", 1);
	free(*line);
	*line = NULL;
}

static int	write_heredoc_loop(int fd, char *delim, int quoted, t_data *data)
{
	char	*line;
	size_t	len;

	len = ft_strlen(delim);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: here-document ", STDERR_FILENO);
			ft_putstr_fd("delimited by end-of-file (wanted `", STDERR_FILENO);
			ft_putstr_fd(delim, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			break ;
		}
		if (ft_strncmp(line, delim, len + 1) == 0)
		{
			free(line);
			break ;
		}
		process_heredoc_line(fd, &line, quoted, data);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, int quoted, t_data *data)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	write_heredoc_loop(pipefd[1], delimiter, quoted, data);
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		close(pipefd[0]);
		return (-1);
	}
	close(pipefd[0]);
	return (0);
}
