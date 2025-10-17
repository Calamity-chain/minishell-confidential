/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:20:16 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/09 17:20:18 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/minishell.h"

int	ft_isquote(char c)
{
	return (c == '\'' || c == '"');
}

int	ft_is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || 
		c == '(' || c == ')' || c == '$' || 
		ft_isquote(c) || ft_isspace(c));
}

int	ft_handle_quote(const char *str, char quote_type)
{
	int	len;

	len = 1;
	while (str[len] && str[len] != quote_type)
		len++;
	if (str[len] == quote_type)
		return (len + 1);
	else
		return (-1);
}

int	ft_get_env_var_length(const char *s)
{
	int	len;

	if (s[0] != '$')
		return (0);
	if (s[1] == '?')
		return (2);
	if (ft_isalpha(s[1]) || s[1] == '_')
	{
		len = 2;
		while (ft_isalnum(s[len]) || s[len] == '_')
			len++;
		return (len);
	}
	return (0);
}
