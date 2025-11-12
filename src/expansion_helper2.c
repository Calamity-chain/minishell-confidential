#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/parser.h"

char	*expand_env_variable_in_string(t_data *data, const char *str)
{
	char	*res;

	if (!str || !data)
		return (NULL);
	res = expand_with_quotes(data, str, 0);
	return (res);
}

char	*handle_quoted_env(t_data *data, t_token *token, char **qstr)
{
	char	*val;
	char	*j;

	if (!qstr || !*qstr)
		return (NULL);
	val = expand_env_variable(data, token->value);
	if (!val)
		return (*qstr);
	j = ft_strjoin(*qstr, val);
	free(val);
	if (!j)
		return (NULL);
	free(*qstr);
	return (j);
}
