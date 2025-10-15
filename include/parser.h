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
# include "lexer.h" // t_token, t_token_type, ft_tokenize
#include "../lib/libft/include/libft.h"

typedef struct s_command {
    char    **args;           // argv, args[0] is command name
    int     *arg_quoted;      // Array of quote flags for each argument
    char    *input_file;      // Input redirection file <
    char    *output_file;     // Output redirection file >
    int     append_mode;      // For >> redirection
    char    *heredoc_delim;    // << delimiter
    int     heredoc_quoted;    // 0/1: is there delimiter in quotes
    int     pipe_output;       // 0/1: if there is | on the right
    struct s_command *next;   // For pipelines
} t_command;

typedef struct s_arglist 
{
    char            *arg;
    struct s_arglist *next;
}   t_arglist;


typedef struct s_quotedlist 
{
    int					quoted;
    struct s_quotedlist	*next;
}   t_quotedlist;

// main parsing functions //
t_command	*parse_pipeline(t_token *tok_head);
t_command	*parse_command(t_token **current);
int		parse_redirection(t_token **current, t_command *cmd);

// quote list management //
int		quotedlist_push_back(t_quotedlist **head, int quoted);
void	quotedlist_clear(t_quotedlist **head);
int		*quotedlist_to_array(t_quotedlist *head);

// helpers //
int			is_redirection(t_token_type type);
int			is_pipe(t_token_type type);
void	    skip_spaces(t_token **token);
int	        is_arg_token(t_token_type type);
const char	*display_token(t_token *token);
char	    **arglist_to_argv(t_arglist *head);
int	        arg_push_back(t_arglist **head, char *arg);
void	    arglist_clear(t_arglist **head, int free_strings);
char	    *join_segments(char *acc, const char *segment);
char	    *parse_one_arg(t_token **current);

#endif
