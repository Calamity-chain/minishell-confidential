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

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

static void	free_redirection_if_exists(t_command *cmd, t_token_type type)
{
	if (type == REDIRECT_IN && cmd->input_file)
	{
		free(cmd->input_file);
		cmd->input_file = NULL;
	}
	else if ((type == REDIRECT_OUT || type == APPEND_OUT) && cmd->output_file)
	{
		free(cmd->output_file);
		cmd->output_file = NULL;
		cmd->append_mode = 0;
	}
	else if (type == HEREDOC && cmd->heredoc_delim)
	{
		free(cmd->heredoc_delim);
		cmd->heredoc_delim = NULL;
		cmd->heredoc_quoted = 0;
	}
}

t_command	*parse_command(t_token **current)
{
	t_command	*cmd;
	t_token		*token;
	t_arglist	*arglist;
	t_quotedlist *quotedlist;
	char		*arg;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->arg_quoted = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delim = NULL;
	cmd->heredoc_quoted = 0;
	cmd->pipe_output = 0;
	cmd->from_env_var = 0;  // NEW: Initialize to 0
	cmd->next = NULL;
	
	arglist = NULL;
	quotedlist = NULL;
	
	token = *current;
	skip_spaces(&token);

	// Parse initial redirections
	while (token && token->type != END_OF_FILE && is_redirection(token->type))
	{
		// Free previous redirection of same type if it exists
		free_redirection_if_exists(cmd, token->type);
		
		if (parse_redirection(&token, cmd) != 0)
		{
			arglist_clear(&arglist, 1);
			quotedlist_clear(&quotedlist);
			free_command(cmd);
			return (NULL);
		}
		skip_spaces(&token);
	}
	
	// Parse command arguments and mixed redirections
	while (token && token->type != END_OF_FILE && !is_pipe(token->type))
	{
		if (token->type == SPACES)
		{
			skip_spaces(&token);
			continue ;
		}
		if (is_redirection(token->type))
		{
			// Free previous redirection of same type if it exists
			free_redirection_if_exists(cmd, token->type);
			
			if (parse_redirection(&token, cmd) != 0)
			{
				arglist_clear(&arglist, 1);
				quotedlist_clear(&quotedlist);
				free_command(cmd);
				return (NULL);
			}
			skip_spaces(&token);
			continue ;
		}
		if (!is_arg_token(token->type))
		{
			printf("syntax error near unexp token `%s`\n", display_token(token));
			arglist_clear(&arglist, 1);
			quotedlist_clear(&quotedlist);
			free_command(cmd);
			return (NULL);
		}
		
		{
			t_token *start_token;
			t_token *temp;
			int overall_quoted;
			
			start_token = token;
			arg = parse_one_arg(&token);
			if (!arg || arg_push_back(&arglist, arg) != 0)
			{
				if (arg)
					free(arg);
				arglist_clear(&arglist, 1);
				quotedlist_clear(&quotedlist);
				free_command(cmd);
				return (NULL);
			}
			
			overall_quoted = start_token->quoted;
			temp = start_token->next;
			while (temp && temp != token && is_arg_token(temp->type))
			{
				if (temp->quoted == Q_SQUOTE)
					overall_quoted = Q_SQUOTE;
				temp = temp->next;
			}
			
			if (quotedlist_push_back(&quotedlist, overall_quoted) != 0)
			{
				arglist_clear(&arglist, 1);
				quotedlist_clear(&quotedlist);
				free_command(cmd);
				return (NULL);
			}
		}
		
		skip_spaces(&token);
	}
	
	if (!arglist && !cmd->input_file && !cmd->output_file && !cmd->heredoc_delim)
	{
		printf("syntax error near unexp token `newline`\n");
		free_command(cmd);
		return (NULL);
	}
	
	cmd->args = arglist_to_argv(arglist);
	// extra CLEANUP:
	if (!cmd->args && arglist) {
    	arglist_clear(&arglist, 1);  // Free strings since they won't be used
	}
	
	if (arglist)
		cmd->arg_quoted = quotedlist_to_array(quotedlist);
	
	arglist_clear(&arglist, 0);
	quotedlist_clear(&quotedlist);
	*current = token;
	return (cmd);
}
