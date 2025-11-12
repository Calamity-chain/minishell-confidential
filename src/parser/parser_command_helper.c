/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:56:49 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/10 00:03:25 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

int	*quotedlist_to_array(t_quotedlist *head)
{
	size_t			count;
	size_t			i;
	int				*array;
	t_quotedlist	*temp;

	count = 0;
	temp = head;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	array = (int *)malloc(sizeof(int) * (count + 1));
	if (!array)
		return (NULL);
	i = 0;
	temp = head;
	while (i < count)
	{
		array[i] = temp->quoted;
		temp = temp->next;
		i++;
	}
	array[count] = -1;
	return (array);
}

int	is_arg_token(t_token_type type)
{
	return (type == WORD || type == STRING_LITERAL || type == ENV_VAR);
}

const char	*display_token(t_token *token)
{
	if (!token || token->type == END_OF_FILE)
		return ("newline");
	if (token->value && token->value[0] != '\0')
		return (token->value);
	if (token->type == PIPE)
		return ("|");
	if (token->type == HEREDOC)
		return ("<<");
	if (token->type == APPEND_OUT)
		return (">>");
	if (token->type == REDIRECT_OUT)
		return (">");
	if (token->type == REDIRECT_IN)
		return ("<");
	return ("token");
}

char	**arglist_to_argv(t_arglist *head)
{
	size_t		count;
	size_t		i;
	char		**argv;
	t_arglist	*temp;

	count = 0;
	temp = head;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	argv = (char **)malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	i = 0;
	temp = head;
	while (i < count)
	{
		argv[i] = temp->arg;
		temp = temp->next;
		i++;
	}
	argv[count] = NULL;
	return (argv);
}

int	arg_push_back(t_arglist **head, char *arg)
{
	t_arglist	*new_node;
	t_arglist	*cursor;

	new_node = (t_arglist *)malloc(sizeof(t_arglist));
	if (!new_node)
		return (-1);
	new_node->arg = arg;
	new_node->next = NULL;
	if (!*head)
	{
		*head = new_node;
		return (0);
	}
	cursor = *head;
	while (cursor->next)
		cursor = cursor->next;
	cursor->next = new_node;
	return (0);
}
