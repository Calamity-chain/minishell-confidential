/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 14:13:44 by rmei              #+#    #+#             */
/*   Updated: 2025/06/01 20:45:45 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Creates a new token.
 * @param type	(t_token_type) The type of the token.
 * @param value	(const char *) The string value of the token.
 * @param len	(size_t) The length of the token in the original input string.
 * @return Pointer to the newly created t_token, or NULL on allocation failure.
 */
static t_token	*ft_make_token(t_token_type type, const char *value, size_t len)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->len = len;
	token->value = NULL;
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
static int	ft_append_token(
	t_list **head, t_token_type type, const char *value, size_t len)
{
	t_token	*new_token;
	t_list	*new_node;

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
	ft_lstadd_back(head, new_node);
	return (1);
}

/**
 * @brief Skips whitespace characters in the input string.
 * @param input	(const char **) Current position in the input string.
 */
static void	ft_skip_whitespace(const char **input)
{
	while (**input && ft_isspace(**input))
		(*input)++;
}

/**
 * @brief Frees the content of a token.
 */
void	ft_free_token(void *token_ptr)
{
	t_token	*token;

	if (!token_ptr)
		return;
	token = (t_token *)token_ptr;
	free(token->value);
	token->value = NULL;
	free(token);
}

/**
 * @brief Tokenizes the input_line and returns a list of tokens.
 * @param input_line	(const char *) The raw command line string.
 * @return A t_list of t_token structures, or NULL on failure/empty input.
 */
t_list	*ft_tokenize(const char *input_line)
{
	const char	*current_char;
	t_list		*tokens;
	size_t		len;

	if (!input_line)
		return (NULL);
	tokens = NULL;
	current_char = input_line;
	printf("Tokenizer: Initializing (input: '%s')\n", input_line);

	while (*current_char)
	{
		ft_skip_whitespace(&current_char);
		if (!*current_char)
			break;
		len = 0;
		// This is where operator/quote handlers would go first
		// For now, default to TOKEN_WORD if no other handler matches
		while (current_char[len] && !ft_isspace(current_char[len])) 
		{
			// TODO: Add checks for operators, quotes etc. to break this word token earlier
			len++;
		}

		if (len > 0) // Found a word
		{
			if (!ft_append_token(&tokens, WORD, current_char, len))
			{
				ft_lstclear(&tokens, ft_free_token);
				return (NULL);
			}
			current_char += len;
		}
		// If len is 0 here, it means ft_isspace was true or end of string, handled by loop condition or ft_skip_whitespace.
		// Or, it could be an unhandled special character if we had other handlers.
	}

	if (!ft_append_token(&tokens, END_OF_FILE, NULL, 0))
	{
		ft_lstclear(&tokens, ft_free_token);
		return (NULL);
	}
	return (tokens);
} 
