#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/parser.h"

char	*expand_single_variable(t_data *data,
			const char *var_start, const char *var_end)
{
	char	*var_name;
	char	*expanded;
	size_t	len;

	if (!var_start || !var_end || var_start >= var_end)
		return (ft_strdup(""));
	len = var_end - var_start;
	var_name = ft_substr(var_start, 0, len);
	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 2) == 0)
		expanded = ft_itoa(data->exit_status);
	else
		expanded = ft_getenv(data, var_name);
	free(var_name);
	if (expanded)
		return (ft_strdup(expanded));
	return (ft_strdup(""));
}

int	append_char(char **res, char c)
{
	char	*tmp;
	size_t	len;

	len = ft_strlen(*res);
	tmp = (char *)malloc(len + 2);
	if (!tmp)
		return (-1);
	ft_memcpy(tmp, *res, len);
	tmp[len] = c;
	tmp[len + 1] = '\0';
	free(*res);
	*res = tmp;
	return (0);
}

static const char	*scan_var_end(const char *s)
{
	if (*s == '?')
		return (s + 1);
	while (ft_isalnum(*s) || *s == '_')
		s++;
	return (s);
}

int	append_var(t_data *data, char **res, const char **pcur, int in_squotes)
{
	const char	*start;
	const char	*end;
	char		*val;
	char		*tmp;

	if (in_squotes || **pcur != '$')
		return (0);
	if (!ft_isalnum((*pcur)[1]) && (*pcur)[1] != '?' && (*pcur)[1] != '_')
		return (0);
	start = *pcur + 1;
	end = scan_var_end(start);
	val = expand_single_variable(data, start, end);
	if (!val)
		return (-1);
	tmp = ft_strjoin(*res, val);
	free(val);
	if (!tmp)
		return (-1);
	free(*res);
	*res = tmp;
	*pcur = end;
	return (1);
}

char	*expand_with_quotes(t_data *data, const char *str, int in_single_quotes)
{
	char		*res;
	const char	*cur;
	int			h;

	if (!str || !data)
		return (NULL);
	res = ft_strdup("");
	if (!res)
		return (NULL);
	cur = str;
	while (*cur)
	{
		h = append_var(data, &res, &cur, in_single_quotes);
		if (h < 0)
			return (free(res), NULL);
		if (h == 0)
		{
			if (append_char(&res, *cur) != 0)
				return (free(res), NULL);
			cur++;
		}
	}
	return (res);
}
