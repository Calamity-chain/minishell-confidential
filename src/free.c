/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:19:35 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/09 17:19:37 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/minishell.h"

void	free_pipeline(t_command *head)
{
	t_command	*n;

	while (head)
	{
		n = head->next;
		free_command(head);
		head = n;
	}
}

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

void	ft_free_matrix(char **matrix)
{
	int	i;

	if (!matrix)
		return ;
	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		i++;
	}
	free(matrix);
}

//UPDATED WITH NEW ARCHITECTURE
void free_command(t_command *cmd)
{
    if (!cmd)
        return;
    
    if (cmd->args)
        ft_free_matrix(cmd->args);
    if (cmd->arg_quoted)
        free(cmd->arg_quoted);
    
    // Free the redirection list
    clear_redirections(&cmd->redirections);
    
    free(cmd);
}
