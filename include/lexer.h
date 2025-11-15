/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:52:21 by ltoscani          #+#    #+#             */
/*   Updated: 2025/11/15 16:54:16 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stddef.h>
# include "libft.h"

typedef enum e_quoted {
	Q_NONE = 0,
	Q_SQUOTE,
	Q_DQUOTE,
}	t_quoted;

typedef enum e_token_type
{
	WORD,
	STRING_LITERAL,
	ENV_VAR,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND_OUT,
	LPARENTHESIS,
	RPARENTHESIS,
	END_OF_FILE,
	SPACES,
	ERROR
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	size_t			len;
	struct s_token	*next;
	struct s_token	*prev;
	t_quoted		quoted;
}	t_token;

// Function prototypes
t_token			*ft_make_token(t_token_type type, const char *value, 
					size_t len);
int				ft_append_token(t_list **head, t_token_type type, 
					const char *value, size_t len);
t_list			*ft_tokenize(const char *input_line);
void			ft_free_token(void *token_ptr);

// helpers
int				ft_isquote(char c);
int				ft_is_special_char(char c);
int				ft_handle_quote(const char *str, char quote_type);
int				ft_get_env_var_length(const char *s);
int				ft_get_operator_length(const char *str);
t_token_type	ft_get_operator_type(const char *str, int len);
int				emit_spaces(const char **cur, t_list **tokens);
int				set_last_quoted(t_list *tokens, char q);
int				emit_quoted(const char **cur, t_list **tokens);
int				emit_env(const char **cur, t_list **tokens);
int				emit_word(const char **cur, t_list **tokens);
int				emit_eof(t_list **tokens);
int				emit_operator(const char **cur, t_list **tokens);

#endif
