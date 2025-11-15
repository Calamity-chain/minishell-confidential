/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirect_helper.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 16:57:16 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/15 16:57:18 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

char	*process_redirection_filename(const char *filename)
{
	size_t	len;
	char	first;

	if (!filename)
		return (NULL);
	len = ft_strlen(filename);
	if (len < 2)
		return (ft_strdup(filename));
	first = filename[0];
	if ((first == '\'' || first == '"') && filename[len - 1] == first)
		return (ft_substr(filename, 1, len - 2));
	return (ft_strdup(filename));
}

void	print_syntax_error(const char *token)
{
	if (!token || !*token)
		token = "newline";
	printf("syntax error near unexp. token `%s`\n", token);
}

int	is_filename_token(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == WORD)
		return (1);
	if (t->type == STRING_LITERAL)
		return (1);
	if (t->type == ENV_VAR)
		return (1);
	return (0);
}

void	fill_redir_flags(t_token_type type, t_token *tok_after,
				int *append_mode, int *heredoc_quoted)
{
	*append_mode = 0;
	*heredoc_quoted = 0;
	if (type == APPEND_OUT)
		*append_mode = 1;
	if (type == HEREDOC && tok_after && tok_after->type == STRING_LITERAL)
		*heredoc_quoted = 1;
}
