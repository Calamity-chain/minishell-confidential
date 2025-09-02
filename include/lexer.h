/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:12:41 by rmei              #+#    #+#             */
/*   Updated: 2025/06/01 20:45:21 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
#include "../lib/libft/include/libft.h"

/**
 * @brief Valid token types for the minishell lexer.
 */
typedef enum e_token_type
{
	WORD,        	/**< e.g., command, argument, filename */
	PIPE,           /**< | */
	REDIRECT_IN,    /**< < */
	REDIRECT_OUT,   /**< > */
	HEREDOC,        /**< << */
	APPEND_OUT,     /**< >> */
	SINGLE_QUOTE,   /**< '...' */
	DOUBLE_QUOTE,   /**< "..." */
    STRING_LITERAL, /**< "hello" or 'world' */
	ENV_VAR,        /**< $VAR or $? */
	LOGICAL_AND,    /**< && */
	LOGICAL_OR,     /**< || */
	LPARENTHESIS,   /**< ( */
	RPARENTHESIS,   /**< ) */
	WILDCARD,       /**< * */
	WHITESPACE,     /**< To be consumed/ignored between tokens */
	END_OF_FILE,    /**< End of input */
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
}   t_token;

// Function prototypes
t_list  *ft_tokenize(const char *input_line);
void    ft_free_token(void *token_ptr); // For ft_lstclear
// const char *ft_token_type_to_string(t_token_type type); // For debugging

#endif
