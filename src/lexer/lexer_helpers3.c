/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:20:29 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/09 17:20:31 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/minishell.h"

int	emit_env(const char **cur, t_list **tokens)
{
	int	len;

	if (**cur != '$')
		return (0);
	len = ft_get_env_var_length(*cur);
	if (len <= 0)
	{
		if (!ft_append_token(tokens, WORD, *cur, 1))
			return (-1);
		*cur += 1;
		return (1);
	}
	if (!ft_append_token(tokens, ENV_VAR, *cur, len))
		return (-1);
	*cur += len;
	return (1);
}

int	emit_word(const char **cur, t_list **tokens)
{
	int	len;

	len = 0;
	while ((*cur)[len] && !ft_is_special_char((*cur)[len]))
		len++;
	if (len <= 0)
		return (0);
	if (!ft_append_token(tokens, WORD, *cur, len))
		return (-1);
	*cur += len;
	return (1);
}

int	emit_eof(t_list **tokens)
{
	if (!ft_append_token(tokens, END_OF_FILE, NULL, 0))
		return (-1);
	return (0);
}

int	emit_operator(const char **cur, t_list **tokens)
{
	int				len;
	t_token_type	type;

	len = ft_get_operator_length(*cur);
	if (len <= 0)
		return (0);
	type = ft_get_operator_type(*cur, len);
	if (!ft_append_token(tokens, type, *cur, len))
		return (-1);
	*cur += len;
	return (1);
}
