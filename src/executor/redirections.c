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

int	handle_input_redirection(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
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

int	handle_output_redirection(char *filename, int append_mode)
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
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
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

int setup_redirections_with_data(t_command *cmd, t_data *data)
{
    t_redirection *redir;
    int result;

    // First pass: Validate all redirections
    redir = cmd->redirections;
    while (redir)
    {
        // Check input file existence
        if ((redir->type == REDIRECT_IN || redir->type == HEREDOC) && 
            access(redir->filename, F_OK) == -1)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(redir->filename, STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            return (1);
        }
        
        // Check output file permissions (try to create/open)
        if ((redir->type == REDIRECT_OUT || redir->type == APPEND_OUT))
        {
            int flags = O_WRONLY | O_CREAT | (redir->append_mode ? O_APPEND : O_TRUNC);
            int test_fd = open(redir->filename, flags, 0644);
            if (test_fd == -1)
            {
                ft_putstr_fd("minishell: ", STDERR_FILENO);
                ft_putstr_fd(redir->filename, STDERR_FILENO);
                ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
                return (1);
            }
            close(test_fd);
        }
        redir = redir->next;
    }

    // Second pass: Apply redirections (last one of each type wins)
    int input_applied = 0;
    int output_applied = 0;
    
    redir = cmd->redirections;
    while (redir)
    {
        // Apply the LAST input redirection
        if ((redir->type == REDIRECT_IN || redir->type == HEREDOC) && !input_applied)
        {
            // Walk to find the last input redirection
            t_redirection *last_input = redir;
            t_redirection *temp = redir->next;
            while (temp)
            {
                if (temp->type == REDIRECT_IN || temp->type == HEREDOC)
                    last_input = temp;
                temp = temp->next;
            }
            
            // Apply the last input redirection
            if (last_input->type == HEREDOC)
                result = handle_heredoc(last_input->filename, last_input->heredoc_quoted, data);
            else
                result = handle_input_redirection(last_input->filename);
            
            if (result != 0)
                return (1);
            
            input_applied = 1;
        }
        
        // Apply the LAST output redirection  
        if ((redir->type == REDIRECT_OUT || redir->type == APPEND_OUT) && !output_applied)
        {
            // Walk to find the last output redirection
            t_redirection *last_output = redir;
            t_redirection *temp = redir->next;
            while (temp)
            {
                if (temp->type == REDIRECT_OUT || temp->type == APPEND_OUT)
                    last_output = temp;
                temp = temp->next;
            }
            
            // Apply the last output redirection
            result = handle_output_redirection(last_output->filename, last_output->append_mode);
            if (result != 0)
                return (1);
            
            output_applied = 1;
        }
        
        redir = redir->next;
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
