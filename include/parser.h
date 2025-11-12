/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:35:38 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/10 01:06:37 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stddef.h>
# include "lexer.h"
# include "libft.h"

typedef struct s_redirection
{
	char					*filename;
	int						type;
	int						append_mode;
	int						heredoc_quoted;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char					**args;
	int						*arg_quoted;
	t_redirection			*redirections;
	int						pipe_output;
	int						from_env_var;
	struct s_command		*next;
}	t_command;

typedef struct s_arglist
{
	char				*arg;
	struct s_arglist	*next;
}	t_arglist;

typedef struct s_quotedlist
{
	int					quoted;
	struct s_quotedlist	*next;
}	t_quotedlist;

/* main parsing functions */
t_command		*parse_pipeline(t_token *tok_head);
t_command		*parse_command(t_token **current);
int				parse_redirection(t_token **current, t_command *cmd);

/* redirection list management */
t_redirection	*create_redirection(char *filename, t_token_type type,
					int append_mode, int heredoc_quoted);
int				add_redirection(t_redirection **head, t_redirection *new_redir);
void			clear_redirections(t_redirection **head);
void			free_redirection(t_redirection *redir);
char			*process_redirection_filename(const char *filename);
void			print_syntax_error(const char *token);
int				is_filename_token(t_token *t);
void			fill_redir_flags(t_token_type type, t_token *tok_after,
					int *append_mode, int *heredoc_quoted);
int				parse_leading_redirs(t_token **token, t_command *cmd);
int				syntax_err_token(t_token *cur);
int				handle_space_or_redir(t_token **cur, t_command *cmd);

/* quote list management */
int				quotedlist_push_back(t_quotedlist **head, int quoted);
void			quotedlist_clear(t_quotedlist **head);
int				*quotedlist_to_array(t_quotedlist *head);

/* helpers */
int				is_redirection(t_token_type type);
int				is_pipe(t_token_type type);
void			skip_spaces(t_token **token);
int				is_arg_token(t_token_type type);
const char		*display_token(t_token *token);
char			**arglist_to_argv(t_arglist *head);
int				arg_push_back(t_arglist **head, char *arg);
void			arglist_clear(t_arglist **head, int free_strings);
char			*join_segments(char *acc, const char *segment);
char			*parse_one_arg(t_token **current);

#endif