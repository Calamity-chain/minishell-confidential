/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:50:23 by asalniko          #+#    #+#             */
/*   Updated: 2025/10/10 01:05:17 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../lib/libft/include/libft.h"


// Helper function to expand a single variable
static char	*expand_single_variable(t_data *data, const char *var_start, const char *var_end)
{
	char	*var_name;
	char	*expanded_value;
	size_t	len;

	if (!var_start || !var_end || var_start >= var_end)
		return (NULL);
	
	len = var_end - var_start;
	var_name = ft_substr(var_start, 0, len);
	if (!var_name)
		return (NULL);
	
	if (ft_strncmp(var_name, "?", 2) == 0)
		expanded_value = ft_itoa(data->exit_status);
	else
		expanded_value = ft_getenv(data, var_name);
	
	free(var_name);
	
	if (expanded_value)
		return (ft_strdup(expanded_value));
	return (ft_strdup(""));
}

// Expand variables in a string with proper quote handling
static char	*expand_with_quotes(t_data *data, const char *str, int in_single_quotes)
{
	char	*result;
	char	*current;
	char	*var_start;
	char	*var_end;
	char	*temp;
	char	*expanded_var;

	if (!str || !data)
		return (NULL);
	
	result = ft_strdup("");
	current = (char *)str;
	
	while (*current)
	{
		// Only expand $ if we're not inside single quotes
		if (*current == '$' && !in_single_quotes && 
			(ft_isalnum(*(current + 1)) || *(current + 1) == '?' || *(current + 1) == '_'))
		{
			var_start = current + 1;
			var_end = var_start;
			
			if (*var_end == '?')
				var_end++;
			else
				while (ft_isalnum(*var_end) || *var_end == '_')
					var_end++;
			
			expanded_var = expand_single_variable(data, var_start, var_end);
			if (expanded_var)
			{
				temp = ft_strjoin(result, expanded_var);
				free(result);
				free(expanded_var);
				result = temp;
				if (!result)
					return (NULL);
			}
			
			current = var_end;
		}
		else if (*current == '\'' || *current == '"')
		{
			// Skip quotes - they will be removed later
			current++;
		}
		else
		{
			// Copy regular character
			temp = malloc(ft_strlen(result) + 2);
			if (!temp)
			{
				free(result);
				return (NULL);
			}
			ft_strlcpy(temp, result, ft_strlen(result) + 1);
			temp[ft_strlen(result)] = *current;
			temp[ft_strlen(result) + 1] = '\0';
			free(result);
			result = temp;
			current++;
		}
	}
	
	return (result);
}

// Main expansion function - now simpler since quote context is handled by caller
char	*expand_env_variable_in_string(t_data *data, const char *str)
{
	char	*result;

	if (!str || !data)
		return (NULL);
	
	// Just expand all variables - the caller ensures we're not in single quotes
	result = expand_with_quotes(data, str, 0);
	
	return (result);
}

char	*handle_quoted_env(t_data *data, t_token *token, char **quoted_string)
{
    char	*expanded_value;
    char	*joined;

    if (!quoted_string || !*quoted_string)
        return (NULL);
    expanded_value = expand_env_variable(data, token->value);
    if (!expanded_value)
        return (*quoted_string);
    joined = ft_strjoin(*quoted_string, expanded_value);
    free(expanded_value);
    if (!joined)
        return (NULL);
    free(*quoted_string);
    return (joined);
}

void	handle_env(t_data *data, t_command *command, int *ac, t_token *token)
{
    char	*expanded_value;

    (void)command;
    (void)ac;
    expanded_value = expand_env_variable(data, token->value);
    if (!expanded_value)
        return ;
    free(token->value);
    token->value = expanded_value;
}

char	*expand_tilda(t_data *data, const char *env_name)
{
    const char	*home;
    const char	*suffix;
    char		*result;
    size_t		home_len;
    size_t		suffix_len;

    if (!env_name)
        return (NULL);
    if (env_name[1] != '/' && env_name[1] != '\0')
        return (ft_strdup(env_name));
    home = ft_getenv(data, "HOME");
    if (!home)
        return (ft_strdup(env_name));
    suffix = env_name + 1;
    home_len = ft_strlen(home);
    suffix_len = ft_strlen(suffix);
    result = (char *)malloc(home_len + suffix_len + 1);
    if (!result)
        return (NULL);
    ft_memcpy(result, home, home_len);
    ft_memcpy(result + home_len, suffix, suffix_len);
    result[home_len + suffix_len] = '\0';
    return (result);
}

char	*expand_env_variable(t_data *data, const char *env_name)
{
    const char	*value;

    if (!data || !env_name || env_name[0] == '\0')
        return (NULL);
    
    // Use ft_strncmp instead of ft_strcmp
    if (ft_strncmp(env_name, "$?", 3) == 0)
        return (ft_itoa(data->exit_status));
    
    if (env_name[0] == '~')
        return (expand_tilda(data, env_name));
    
    value = ft_getenv(data, env_name);
    if (!value)
        return (NULL);
    return (ft_strdup(value));
}

char	*ft_getenv(t_data *data, const char *env_name)
{
	size_t		name_len;
	const char	*name_start;
	int			i;

	if (!data || !data->env || !env_name)
		return (NULL);
	name_start = env_name;
	if (*name_start == '$')
		name_start++;
	name_len = ft_strlen(name_start);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], name_start, name_len) == 0
			&& data->env[i][name_len] == '=')
			return (data->env[i] + name_len + 1);
		i++;
	}
	return (NULL);
}
