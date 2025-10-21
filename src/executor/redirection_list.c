/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 19:01:06 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/21 19:01:49 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* NEW FILE CREATED FOR THE NEW ARCHITECTURE IMPLEMENTATION to handle redirection issues */

#include "parser.h"
#include <stdlib.h>

t_redirection *create_redirection(char *filename, t_token_type type, int append_mode, int heredoc_quoted)
{
    t_redirection *new_redir;

    new_redir = malloc(sizeof(t_redirection));
    if (!new_redir)
        return (NULL);
    
    new_redir->filename = filename;
    new_redir->type = type;
    new_redir->append_mode = append_mode;
    new_redir->heredoc_quoted = heredoc_quoted;
    new_redir->next = NULL;
    
    return (new_redir);
}

int add_redirection(t_redirection **head, t_redirection *new_redir)
{
    t_redirection *current;

    if (!new_redir)
        return (-1);
    
    if (!*head)
    {
        *head = new_redir;
        return (0);
    }
    
    current = *head;
    while (current->next)
        current = current->next;
    
    current->next = new_redir;
    return (0);
}

void clear_redirections(t_redirection **head)
{
    t_redirection *current;
    t_redirection *next;

    current = *head;
    while (current)
    {
        next = current->next;
        free_redirection(current);
        current = next;
    }
    *head = NULL;
}

void free_redirection(t_redirection *redir)
{
    if (!redir)
        return;
    
    free(redir->filename);
    free(redir);
}
