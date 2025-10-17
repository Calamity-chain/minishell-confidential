/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command_helper2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 23:56:55 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/10 00:11:02 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

static char	*remove_outer_quotes(const char *str)
{
	char	*result;
	size_t	len;
	char	first_char;

	if (!str || ft_strlen(str) < 2)
		return (ft_strdup(str));
	
	first_char = str[0];
	len = ft_strlen(str);
	
	// Only remove if the first and last characters are matching quotes
	if ((first_char == '\'' || first_char == '"') && str[len - 1] == first_char)
	{
		// Remove both outer quotes
		result = ft_substr(str, 1, len - 2);
	}
	else
	{
		result = ft_strdup(str);
	}
	
	return (result);
}

char	*join_segments(char *acc, const char *segment)
{
	char	*res;
	size_t	len_acc;
	size_t	len_seg;

	len_acc = 0;
	if (acc)
		len_acc = ft_strlen(acc);
	len_seg = 0;
	if (segment)
		len_seg = ft_strlen(segment);
	res = (char *)malloc(len_acc + len_seg + 1);
	if (!res)
	{
		free(acc);
		return (NULL);
	}
	if (acc)
		ft_memcpy(res, acc, len_acc);
	if (segment)
		ft_memcpy(res + len_acc, segment, len_seg);
	res[len_acc + len_seg] = '\0';
	free(acc);
	return (res);
}

char	*parse_one_arg(t_token **current)
{
	t_token	*cursor;
	char	*arg;

	cursor = *current;
	arg = NULL;
	if (!cursor || !is_arg_token(cursor->type))
		return (NULL);
	
	// Concatenate all adjacent argument tokens
	while (cursor && is_arg_token(cursor->type))
	{
		// For quoted tokens, we need to remove the quotes during concatenation
		if (cursor->type == STRING_LITERAL && cursor->value)
		{
			char *unquoted = remove_outer_quotes(cursor->value);
			arg = join_segments(arg, unquoted);
			free(unquoted);
		}
		else
		{
			arg = join_segments(arg, cursor->value);
		}
		
		if (!arg)
			return (NULL);
		cursor = cursor->next;
	}
	*current = cursor;
	return (arg);
}
