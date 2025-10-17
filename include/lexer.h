/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:52:21 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/10 01:19:25 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
#include "../lib/libft/include/libft.h"

typedef enum e_quoted {
	Q_NONE = 0,
	Q_SQUOTE,   /* '...' */
	Q_DQUOTE    /* "..." */
}	t_quoted;

/**
 * @brief Valid token types for the minishell lexer.
 */
typedef enum e_token_type
{
	WORD,        	/**< e.g., command, argument, filename */
	STRING_LITERAL, /**< "hello" or 'world' */
	ENV_VAR,        /**< $VAR or $? */
	PIPE,           /**< | */
	REDIRECT_IN,    /**< < */
	REDIRECT_OUT,   /**< > */
	HEREDOC,        /**< << */
	APPEND_OUT,     /**< >> */
	LPARENTHESIS,   /**< ( */
	RPARENTHESIS,   /**< ) */
	END_OF_FILE,    /**< End of input */
	SPACES,		/**< Spaces or tabs */
	ERROR           /**< For lexical errors */
}   t_token_type;

/**
 * @brief A token.
 * @param value (char) Actual value of the token, e.g., "ls", "|", "file.txt"
 * @param type  (t_token_type) Pre-defined token type
 * @param len   (size_t) The length of the token value
 */
typedef struct s_token
{
	char		    *value;
	t_token_type	type;
	size_t			len;
	struct s_token  *next; // Pointer to next token in the list
	struct s_token	*prev; // Pointer to previous token in the list
	t_quoted		quoted; // Quoting state (none, single, double)
}   t_token;

// Function prototypes
t_token	*ft_make_token(t_token_type type, const char *value, size_t len);
int	ft_append_token(t_list **head, t_token_type type, const char *value, size_t len);
t_list  *ft_tokenize(const char *input_line);
void    ft_free_token(void *token_ptr); // For ft_lstclear
// const char *ft_token_type_to_string(t_token_type type); // For debugging

// helpers
int	ft_isquote(char c);
int	ft_is_special_char(char c);
int	ft_handle_quote(const char *str, char quote_type);
int	ft_get_env_var_length(const char *s);
int	ft_get_operator_length(const char *str);
t_token_type	ft_get_operator_type(const char *str, int len);
int	emit_spaces(const char **cur, t_list **tokens);
int	set_last_quoted(t_list *tokens, char q);
int	emit_quoted(const char **cur, t_list **tokens);
int	emit_env(const char **cur, t_list **tokens);
int	emit_word(const char **cur, t_list **tokens);
int	emit_eof(t_list **tokens);
int	emit_operator(const char **cur, t_list **tokens);



#endif
