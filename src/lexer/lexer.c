/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:44:21 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/10 00:53:05 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/minishell.h"

t_token	*ft_make_token(t_token_type type, const char *value, size_t len)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->len = len;
	token->value = NULL;
	token->next = NULL;
	token->prev = NULL;
	token->quoted = Q_NONE;
	if (value && len >= 0)
	{
		token->value = ft_substr(value, 0, len);
		if (!token->value)
		{
			free(token);
			return (NULL);
		}
	}
	return (token);
}

int	ft_append_token(t_list **head, t_token_type type,
	const char *value, size_t len)
{
	t_token	*new_token;
	t_list	*new_node;
	t_list	*last;
	t_token	*last_token;

	new_token = ft_make_token(type, value, len);
	if (!new_token)
		return (0);
	new_node = ft_lstnew(new_token);
	if (!new_node)
	{
		free(new_token->value);
		free(new_token);
		return (0);
	}
	if (*head)
	{
		last = ft_lstlast(*head);
		last_token = (t_token *)last->content;
		last_token->next = new_token;
		new_token->prev = last_token;
	}
	ft_lstadd_back(head, new_node);
	return (1);
}

static int	process_ws_op_quote(const char **cur, t_list **tokens)
{
	int	r;

	r = emit_spaces(cur, tokens);
	if (r < 0)
		return (-1);
	if (r > 0)
		return (1);
	r = emit_operator(cur, tokens);
	if (r < 0)
		return (-1);
	if (r > 0)
		return (1);
	if (ft_isquote(**cur))
	{
		if (emit_quoted(cur, tokens) < 0)
			return (-1);
		return (1);
	}
	return (0);
}

static int	process_env_or_word(const char **cur, t_list **tokens)
{
	int	r;

	r = emit_env(cur, tokens);
	if (r < 0)
		return (-1);
	if (r > 0)
		return (1);
	r = emit_word(cur, tokens);
	if (r < 0)
		return (-1);
	if (r == 0)
		(*cur)++;
	return (1);
}

t_list	*ft_tokenize(const char *input_line)
{
	const char	*cur;
	t_list		*tokens;
	int			s;

	if (!input_line)
		return (NULL);
	tokens = NULL;
	cur = input_line;
	while (*cur)
	{
		s = process_ws_op_quote(&cur, &tokens);
		if (s < 0)
			return (ft_lstclear(&tokens, ft_free_token), NULL);
		if (s == 0 && process_env_or_word(&cur, &tokens) < 0)
			return (ft_lstclear(&tokens, ft_free_token), NULL);
	}
	if (emit_eof(&tokens) < 0)
		return (ft_lstclear(&tokens, ft_free_token), NULL);
	return (tokens);
}
