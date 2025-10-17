/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:12:59 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/10 00:57:31 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

static char	*process_redirection_filename(const char *filename)
{
	size_t	len;
	char	first_char;

	if (!filename)
		return (NULL);
	
	len = ft_strlen(filename);
	if (len < 2)
		return (ft_strdup(filename));
	
	first_char = filename[0];
	
	// Remove surrounding quotes if present
	if ((first_char == '\'' || first_char == '"') && 
		filename[len - 1] == first_char)
	{
		return (ft_substr(filename, 1, len - 2));
	}
	
	return (ft_strdup(filename));
}

static int	assign_target(char **dst, const char *src)
{
	char	*processed;
	char	*dup;

	processed = process_redirection_filename(src);
	if (!processed)
		return (-1);
		
	dup = ft_strdup(processed);
	free(processed);
	
	if (!dup)
		return (-1);
	free(*dst);
	*dst = dup;
	return (0);
}

static void	print_syntax_error(const char *token)
{
	if (!token || !*token)
		token = "newline";
	printf("syntax error near unexp. token `%s`\n", token);
}
/**
 * @brief Concatenate adjacent string tokens for redirection filenames
 */
static char	*concatenate_adjacent_tokens(t_token **token)
{
	char	*result;
	char	*temp;
	t_token	*current;

	if (!token || !*token)
		return (NULL);
	
	current = *token;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	
	// Concatenate all adjacent WORD, STRING_LITERAL, and ENV_VAR tokens
	while (current && (current->type == WORD || current->type == STRING_LITERAL || current->type == ENV_VAR))
	{
		// Process each token (remove quotes if needed)
		char *processed = process_redirection_filename(current->value);
		if (!processed)
		{
			free(result);
			return (NULL);
		}
		
		// Concatenate with previous result
		temp = ft_strjoin(result, processed);
		free(result);
		free(processed);
		
		if (!temp)
			return (NULL);
		
		result = temp;
		current = current->next;
	}
	
	// Update the token pointer to point to the next non-filename token
	*token = current;
	
	return (result);
}

int	parse_redirection(t_token **current, t_command *cmd)
{
	t_token			*token;
	t_token_type	redir_type;
	const char		*bad;

	token = *current;
	skip_spaces(&token);
	if (!token || !is_redirection(token->type))
	{
		bad = NULL;
		if (token)
			bad = token->value;
		print_syntax_error(bad);
		return (-1);
	}
	redir_type = token->type;
	token = token->next;
	skip_spaces(&token);
	if (!token || (token->type != WORD 
			&& token->type != STRING_LITERAL 
			&& token->type != ENV_VAR))
	{
		bad = NULL;
		if (token)
			bad = token->value;
		print_syntax_error(bad);
		return (-1);
	}

	// NEW: Concatenate adjacent string tokens for the filename
	char *full_filename = concatenate_adjacent_tokens(&token);
	
	if (!full_filename)
		return (-1);

	if (redir_type == REDIRECT_IN 
		&& assign_target(&cmd->input_file, full_filename) < 0)
	{
		free(full_filename);
		return (-1);
	}
	if ((redir_type == REDIRECT_OUT || redir_type == APPEND_OUT) 
		&& assign_target(&cmd->output_file, full_filename) < 0)
	{
		free(full_filename);
		return (-1);
	}
	if (redir_type == HEREDOC 
		&& assign_target(&cmd->heredoc_delim, full_filename) < 0)
	{
		free(full_filename);
		return (-1);
	}
	
	free(full_filename);
	
	if (redir_type == APPEND_OUT)
		cmd->append_mode = 1;
	else if (redir_type == REDIRECT_OUT)
		cmd->append_mode = 0;
	if (redir_type == HEREDOC)
		cmd->heredoc_quoted = (token->type == STRING_LITERAL);
	*current = token;
	return (0);
}
