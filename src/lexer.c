/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:44:21 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/15 15:44:37 by ltoscani         ###   ########.fr       */
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
 * @brief Gets the length of an operator token.
 * @param str (const char *) The input string.
 * @return Length of the operator (1 or 2), or 0 if not an operator.
 */
static int	ft_get_operator_length(const char *str)
{
	if (ft_strncmp(str, "<<", 2) == 0) return 2;
	if (ft_strncmp(str, ">>", 2) == 0) return 2;
	if (ft_strncmp(str, "||", 2) == 0) return 2;
	if (ft_strncmp(str, "&&", 2) == 0) return 2;
	if (*str == '|' || *str == '<' || *str == '>' || 
		*str == '(' || *str == ')') return 1;
	return 0;
}

/**
 * @brief Gets the token type for an operator.
 * @param str (const char *) The operator string.
 * @param len (int) Length of the operator.
 * @return The corresponding token type.
 */
static t_token_type	ft_get_operator_type(const char *str, int len)
{
	if (len == 2)
	{
		if (ft_strncmp(str, "<<", 2) == 0) return HEREDOC;
		if (ft_strncmp(str, ">>", 2) == 0) return APPEND_OUT;
		if (ft_strncmp(str, "||", 2) == 0) return LOGICAL_OR;
		if (ft_strncmp(str, "&&", 2) == 0) return LOGICAL_AND;
	}
	else if (len == 1)
	{
		if (*str == '|') return PIPE;
		if (*str == '<') return REDIRECT_IN;
		if (*str == '>') return REDIRECT_OUT;
		if (*str == '(') return LPARENTHESIS;
		if (*str == ')') return RPARENTHESIS;
	}
	return ERROR;
}


/**
 * @brief Handles quoted strings.
 * @param str (const char *) The input string.
 * @param quote_type (char) The type of quote (' or ").
 * @return Length of the quoted string including quotes, or -1 if unclosed.
 */
static int	ft_handle_quote(const char *str, char quote_type)
{
	int len = 1; // Start after the opening quote
	
	while (str[len] && str[len] != quote_type)
		len++;
	
	if (str[len] == quote_type)
		return len + 1; // Include closing quote
	else
		return -1; // Unclosed quote
}

/**
 * @brief Gets the length of an environment variable.
 * @param str (const char *) The input string starting with $.
 * @return Length of the environment variable token.
 */
static int	ft_get_env_var_length(const char *str)
{
	int len = 1; // Start after the '$'
	
	if (str[1] == '?') // Handle $?
		return 2;
	
	if (ft_isalpha(str[1]) || str[1] == '_')
	{
		len++;
		while (ft_isalnum(str[len]) || str[len] == '_')
			len++;
	}
	return len;
}

/**
 * @brief Checks if a character is a quote.
 * @param c (char) The character to check.
 * @return 1 if quote, 0 otherwise.
 */
static int	ft_isquote(char c)
{
	return (c == '\'' || c == '"');
}

/**
 * @brief Checks if a character is a special token character.
 * @param c (char) The character to check.
 * @return 1 if special, 0 otherwise.
 */
static int	ft_is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || 
			c == '(' || c == ')' || c == '$' || 
			ft_isquote(c) || ft_isspace(c));
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
	int			len;
	t_token_type	type;

	if (!input_line)
		return (NULL);
	tokens = NULL;
	current_char = input_line;

	while (*current_char)
	{
		ft_skip_whitespace(&current_char);
		if (!*current_char)
			break;

		// Check for operators first
		len = ft_get_operator_length(current_char);
		if (len > 0)
		{
			type = ft_get_operator_type(current_char, len);
			if (!ft_append_token(&tokens, type, current_char, len))
			{
				ft_lstclear(&tokens, ft_free_token);
				return (NULL);
			}
			current_char += len;
			continue;
		}

		// Check for quotes
		if (ft_isquote(*current_char))
		{
			len = ft_handle_quote(current_char, *current_char);
			if (len < 0)
			{
				ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
				ft_lstclear(&tokens, ft_free_token);
				return (NULL);
			}
			type = (*current_char == '\'') ? SINGLE_QUOTE : DOUBLE_QUOTE;
			if (!ft_append_token(&tokens, type, current_char, len))
			{
				ft_lstclear(&tokens, ft_free_token);
				return (NULL);
			}
			current_char += len;
			continue;
		}

		// Check for environment variables
		if (*current_char == '$')
		{
			len = ft_get_env_var_length(current_char);
			if (!ft_append_token(&tokens, ENV_VAR, current_char, len))
			{
				ft_lstclear(&tokens, ft_free_token);
				return (NULL);
			}
			current_char += len;
			continue;
		}

		// Handle regular words
		len = 0;
		while (current_char[len] && !ft_is_special_char(current_char[len]))
			len++;

		if (len > 0)
		{
			if (!ft_append_token(&tokens, WORD, current_char, len))
			{
				ft_lstclear(&tokens, ft_free_token);
				return (NULL);
			}
			current_char += len;
			continue;
		}
	}

	if (!ft_append_token(&tokens, END_OF_FILE, NULL, 0))
	{
		ft_lstclear(&tokens, ft_free_token);
		return (NULL);
	}
	
	return (tokens);
}
