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

static int	expect_cmd_after_pipe(t_token **tok)
{
	if (!*tok)
		return (-1);
	*tok = (*tok)->next;
	skip_spaces(tok);
	if (!*tok || (*tok)->type == END_OF_FILE || is_pipe((*tok)->type))
	{
		printf("syntax error near unexp. token `|`\n");
		return (-1);
	}
	return (0);
}

static void	append_cmd(t_command **head, t_command **tail, t_command *node)
{
	if (!*head)
		*head = node;
	else
		(*tail)->next = node;
	*tail = node;
}

static int	start_check(t_token **tok)
{
	skip_spaces(tok);
	if (!*tok || (*tok)->type == END_OF_FILE)
		return (-1);
	if (is_pipe((*tok)->type))
	{
		printf("syntax error near unexp. token `|`\n");
		return (-1);
	}
	return (0);
}

static int	build_pipeline(t_token **tok, t_command **head, t_command **tail)
{
	t_command	*node;

	while (*tok && (*tok)->type != END_OF_FILE)
	{
		node = parse_command(tok);
		if (!node)
			return (-1);
		append_cmd(head, tail, node);
		skip_spaces(tok);
		if (*tok && is_pipe((*tok)->type))
		{
			if (expect_cmd_after_pipe(tok) != 0)
				return (-1);
			continue ;
		}
		break ;
	}
	return (0);
}

t_command	*parse_pipeline(t_token *tok_head)
{
	t_command	*head;
	t_command	*tail;
	t_token		*token;

	head = NULL;
	tail = NULL;
	token = tok_head;
	if (start_check(&token) != 0)
		return (NULL);
	if (build_pipeline(&token, &head, &tail) != 0)
		return (free_pipeline(head), NULL);
	skip_spaces(&token);
	if (token && token->type != END_OF_FILE)
		return (printf("syntax error near unexp. token\n"),
			free_pipeline(head), NULL);
	return (head);
}
