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
	if ((first_char == '\'' || first_char == '"')
		&& filename[len - 1] == first_char)
		return (ft_substr(filename, 1, len - 2));
	return (ft_strdup(filename));
}

static void	print_syntax_error(const char *token)
{
	if (!token || !*token)
		token = "newline";
	printf("syntax error near unexp. token `%s`\n", token);
}

static char	*concatenate_adjacent_tokens(t_token **token)
{
	char	*result;
	char	*temp;
	t_token	*current;
	char	*processed;

	if (!token || !*token)
		return (NULL);
	current = *token;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (current && (current->type == WORD
			|| current->type == STRING_LITERAL || current->type == ENV_VAR))
	{
		processed = process_redirection_filename(current->value);
		if (!processed)
		{
			free(result);
			return (NULL);
		}
		temp = ft_strjoin(result, processed);
		free(result);
		free(processed);
		if (!temp)
			return (NULL);
		result = temp;
		current = current->next;
	}
	*token = current;
	return (result);
}

int	parse_redirection(t_token **current, t_command *cmd)
{
	t_token			*token;
	t_token_type	redir_type;
	const char		*bad;
	char			*full_filename;
	t_redirection	*new_redir;

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
	full_filename = concatenate_adjacent_tokens(&token);
	if (!full_filename)
		return (-1);
	new_redir = create_redirection(full_filename, redir_type,
								  (redir_type == APPEND_OUT) ? 1 : 0,
								  (redir_type == HEREDOC && token && token->type == STRING_LITERAL) ? 1 : 0);
	if (!new_redir)
	{
		free(full_filename);
		return (-1);
	}
	if (add_redirection(&cmd->redirections, new_redir) != 0)
	{
		free_redirection(new_redir);
		return (-1);
	}
	*current = token;
	return (0);
}
