/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_pipeline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:26:59 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/09 23:54:33 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

t_command	*parse_pipeline(t_token *tok_head)
{
	t_command	*pipeline_head;
	t_command	*current_cmd;
	t_token		*current_token;
	t_command	*new_cmd;

	pipeline_head = NULL;
	current_cmd = NULL;
	current_token = tok_head;
	skip_spaces(&current_token);
	if (!current_token || current_token->type == END_OF_FILE)
		return (NULL);
	if (is_pipe(current_token->type))
	{
		printf("syntax error near unexp. token `|`\n");
		return (NULL);
	}
	while (current_token && current_token->type != END_OF_FILE)
	{
		new_cmd = parse_command(&current_token);
		if (!new_cmd)
		{
			free_pipeline(pipeline_head);
			return (NULL);
		}
		if (!pipeline_head)
			pipeline_head = new_cmd;
		else
			current_cmd->next = new_cmd;
		current_cmd = new_cmd;
		skip_spaces(&current_token);
		if (current_token && is_pipe(current_token->type))
		{
			current_token = current_token->next;
			skip_spaces(&current_token);
			if (!current_token || current_token->type == END_OF_FILE 
				|| is_pipe(current_token->type))
			{
				printf("syntax error near unexp. token `|`\n");
				free_pipeline(pipeline_head);
				return (NULL);
			}
			continue ;
		}
		break ;
	}
	skip_spaces(&current_token);
	if (current_token && current_token->type != END_OF_FILE)
	{
		printf("syntax error near unexp. token\n");
		free_pipeline(pipeline_head);
		return (NULL);
	}
	return (pipeline_head);
}
