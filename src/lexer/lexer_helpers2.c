/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:20:22 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/09 17:20:24 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/lexer.h"
#include "../../include/minishell.h"

int	ft_get_operator_length(const char *str)
{
	if (ft_strncmp(str, "<<", 2) == 0) 
		return (2);
	if (ft_strncmp(str, ">>", 2) == 0) 
		return (2);
	if (ft_strncmp(str, "||", 2) == 0) 
		return (2);
	if (ft_strncmp(str, "&&", 2) == 0) 
		return (2);
	if (*str == '|' || *str == '<' || *str == '>' || 
		*str == '(' || *str == ')') 
		return (1);
	return (0);
}

t_token_type	ft_get_operator_type(const char *str, int len)
{
	if (len == 2)
	{
		if (ft_strncmp(str, "<<", 2) == 0) 
			return (HEREDOC);
		if (ft_strncmp(str, ">>", 2) == 0) 
			return (APPEND_OUT);
		if (ft_strncmp(str, "||", 2) == 0) 
			return (ERROR);
		if (ft_strncmp(str, "&&", 2) == 0) 
			return (ERROR);
	}
	else if (len == 1)
	{
		if (*str == '|') 
			return (PIPE);
		if (*str == '<') 
			return (REDIRECT_IN);
		if (*str == '>') 
			return (REDIRECT_OUT);
		if (*str == '(') 
			return (LPARENTHESIS);
		if (*str == ')') 
			return (RPARENTHESIS);
	}
	return (ERROR);
}

int	emit_spaces(const char **cur, t_list **tokens)
{
	const char	*p;
	int			len;

	p = *cur;
	len = 0;
	while (p[len] == ' ' || p[len] == '\t')
		len++;
	if (len == 0)
		return (0);
	if (!ft_append_token(tokens, SPACES, (char *)(*cur), len))
		return (-1);
	*cur += len;
	return (1);
}

int	set_last_quoted(t_list *tokens, char q)
{
	t_list	*last;
	t_token	*tk;

	last = ft_lstlast(tokens);
	if (!last || !last->content)
		return (0);
	tk = (t_token *)last->content;
	if (q == '\'')
		tk->quoted = Q_SQUOTE;
	else
		tk->quoted = Q_DQUOTE;
	return (1);
}

int	emit_quoted(const char **cur, t_list **tokens)
{
	char		q;
	int			len;
	const char	*inner;
	int			inner_len;

	q = **cur;
	len = ft_handle_quote(*cur, q);
	if (len < 0)
		return (ft_putstr_fd("minishell: syntax error: unclosed quote\n",
				STDERR_FILENO), -1);
	inner = *cur + 1;
	inner_len = len - 2;
	if (inner_len < 0)
		inner_len = 0;
	if (!ft_append_token(tokens, STRING_LITERAL, (char *)inner, inner_len))
		return (-1);
	if (!set_last_quoted(*tokens, q))
		return (-1);
	*cur += len;
	return (1);
}
