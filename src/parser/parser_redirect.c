/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:12:59 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/15 19:25:27 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

static int	append_processed_value(char **res, const char *value)
{
	char	*proc;
	char	*tmp;

	proc = process_redirection_filename(value);
	if (!proc)
		return (-1);
	tmp = ft_strjoin(*res, proc);
	free(*res);
	free(proc);
	if (!tmp)
		return (-1);
	*res = tmp;
	return (0);
}

static char	*concatenate_adjacent_tokens(t_token **token)
{
	t_token	*cur;
	char	*res;

	if (!token || !*token)
		return (NULL);
	cur = *token;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (cur && (cur->type == WORD || cur->type == STRING_LITERAL
			|| cur->type == ENV_VAR))
	{
		if (append_processed_value(&res, cur->value) != 0)
			return (NULL);
		cur = cur->next;
	}
	*token = cur;
	return (res);
}

static int	syntax_err_tok(t_token *token)
{
	const char	*bad;

	bad = NULL;
	if (token)
		bad = token->value;
	print_syntax_error(bad);
	return (-1);
}

static int	make_and_add_redir(t_command *cmd, t_token_type type,
				char *fname, t_token *after_tok)
{
	int				append_mode;
	int				heredoc_quoted;
	t_redirection	*node;

	fill_redir_flags(type, after_tok, &append_mode, &heredoc_quoted);
	node = create_redirection(fname, type, append_mode, heredoc_quoted);
	if (!node)
	{
		free(fname);
		return (-1);
	}
	if (add_redirection(&cmd->redirections, node) != 0)
	{
		free_redirection(node);
		return (-1);
	}
	return (0);
}

int	parse_redirection(t_token **current, t_command *cmd)
{
	t_token			*token;
	t_token_type	type;
	char			*fname;

	token = *current;
	skip_spaces(&token);
	if (!token || !is_redirection(token->type))
		return (syntax_err_tok(token));
	type = token->type;
	token = token->next;
	skip_spaces(&token);
	if (!is_filename_token(token))
		return (syntax_err_tok(token));
	fname = concatenate_adjacent_tokens(&token);
	if (!fname)
		return (-1);
	if (make_and_add_redir(cmd, type, fname, token) != 0)
		return (-1);
	*current = token;
	return (0);
}
