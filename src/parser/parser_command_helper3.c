#include "../../include/lexer.h"
#include "../../include/parser.h"
#include "../../include/minishell.h"

int	parse_leading_redirs(t_token **token, t_command *cmd)
{
	t_token	*cur;

	cur = *token;
	skip_spaces(&cur);
	while (cur && cur->type != END_OF_FILE && is_redirection(cur->type))
	{
		if (parse_redirection(&cur, cmd) != 0)
			return (-1);
		skip_spaces(&cur);
	}
	*token = cur;
	return (0);
}

int	syntax_err_token(t_token *cur)
{
	printf("syntax error near unexp token `%s`\n", display_token(cur));
	return (-1);
}

int	handle_space_or_redir(t_token **cur, t_command *cmd)
{
	if ((*cur)->type == SPACES)
	{
		skip_spaces(cur);
		return (1);
	}
	if (is_redirection((*cur)->type))
	{
		if (parse_redirection(cur, cmd) != 0)
			return (-1);
		skip_spaces(cur);
		return (1);
	}
	return (0);
}
