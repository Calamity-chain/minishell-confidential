/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 18:11:55 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/10 00:18:55 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

t_command	*parse_command(t_token **current)
{
	t_command	*cmd;
	t_token		*token;
	t_arglist	*arglist;
	char		*arg;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_quoted = 0;
	cmd->pipe_output = 0;
	cmd->next = NULL;
	arglist = NULL;
	token = *current;
	skip_spaces(&token);
	while (token && token->type != END_OF_FILE 
		&& is_redirection(token->type))
	{
		if (parse_redirection(&token, cmd) != 0)
		{
			arglist_clear(&arglist, 1);
			free_command(cmd);
			return (NULL);
		}
		skip_spaces(&token);
	}
	while (token && token->type != END_OF_FILE && !is_pipe(token->type))
	{
		if (token->type == SPACES)
		{
			skip_spaces(&token);
			continue ;
		}
		if (is_redirection(token->type))
		{
			if (parse_redirection(&token, cmd) != 0)
			{
				arglist_clear(&arglist, 1);
				free_command(cmd);
				return (NULL);
			}
			skip_spaces(&token);
			continue ;
		}
		if (!is_arg_token(token->type))
		{
			printf("syntax error near unexp token `%s`\n",
				display_token(token));
			arglist_clear(&arglist, 1);
			free_command(cmd);
			return (NULL);
		}
		arg = parse_one_arg(&token);
		if (!arg || arg_push_back(&arglist, arg) != 0)
		{
			if (arg)
				free(arg);
			arglist_clear(&arglist, 1);
			free_command(cmd);
			return (NULL);
		}
		skip_spaces(&token);
	}
	if (!arglist && !cmd->input_file 
		&& !cmd->output_file && !cmd->heredoc_delim)
	{
		printf("syntax error near unexp token `newline`\n");
		free_command(cmd);
		return (NULL);
	}
	cmd->args = arglist_to_argv(arglist);
	if (!cmd->args)
	{
		arglist_clear(&arglist, 1);
		free_command(cmd);
		return (NULL);
	}
	arglist_clear(&arglist, 0);
	*current = token;
	return (cmd);
}
