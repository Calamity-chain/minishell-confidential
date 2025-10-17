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

/**
 * @brief Creates a new token.
 * @param type	(t_token_type) The type of the token.
 * @param value	(const char *) The string value of the token.
 * @param len	(size_t) The length of the token in the original input string.
 * @return Pointer to the newly created t_token, or NULL on allocation failure.
 */

t_token	*ft_make_token(t_token_type type, const char *value, size_t len)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->len = len;
	token->value = NULL;
	token->next = NULL;  // Initialize next pointer -> FIX TOKEN LINKING
	token->prev = NULL;  // Initialize prev pointer -> FIX TOKEN LINKING
	token->quoted = Q_NONE;  // Initialize quoted state -> FIX TOKEN LINKING
	
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

/**
 * @brief Appends a new token to a linked list.
 * @param head	(t_list **) Pointer to the head of the list.
 * @param type	(t_token_type) The type of the token to add.
 * @param value	(const char *) Pointer to the start of the token's value.
 * @param len	(size_t) The length of the token.
 * @return 1 on success, 0 on failure (e.g., memory allocation error).
 */
int	ft_append_token(t_list **head, t_token_type type, const char *value, size_t len)
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
	// Link the tokens together via the next/prev pointers
	if (*head)
	{
		last = ft_lstlast(*head);
		last_token = (t_token *)last->content;
		last_token->next = new_token;  // Link previous token to new token
		new_token->prev = last_token;  // Link new token back to previous
	}
	ft_lstadd_back(head, new_node);
	return (1);
}

/**
 * @brief Tokenizes the input_line and returns a list of tokens.
 * @param input_line	(const char *) The raw command line string.
 * @return A t_list of t_token structures, or NULL on failure/empty input.
 */
t_list	*ft_tokenize(const char *input_line)
{
	const char	*cur;
	t_list		*tokens;
	int			r;

	if (!input_line)
		return (NULL);
	tokens = NULL;
	cur = input_line;
	while (*cur)
	{
		r = emit_spaces(&cur, &tokens);
		if (r < 0) 
			return (ft_lstclear(&tokens, ft_free_token), NULL);
		if (r > 0) 
			continue ;
		r = emit_operator(&cur, &tokens);
		if (r < 0) 
			return (ft_lstclear(&tokens, ft_free_token), NULL);
		if (r > 0) 
			continue ;
		if (ft_isquote(*cur))
		{
			if (emit_quoted(&cur, &tokens) < 0)
				return (ft_lstclear(&tokens, ft_free_token), NULL);
			continue ;
		}
		r = emit_env(&cur, &tokens);
		if (r < 0) 
			return (ft_lstclear(&tokens, ft_free_token), NULL);
		if (r > 0) 
			continue ;
		r = emit_word(&cur, &tokens);
		if (r < 0) 
			return (ft_lstclear(&tokens, ft_free_token), NULL);
		if (r == 0) 
			cur++;
	}
	if (emit_eof(&tokens) < 0)
	{
		ft_lstclear(&tokens, ft_free_token);
		return (NULL);
	}
	return (tokens);
}
