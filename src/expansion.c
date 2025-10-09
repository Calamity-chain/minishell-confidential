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
        if (!ft_strncmp(data->env[i], name_start, name_len)
            && data->env[i][name_len] == '=')
            return (data->env[i] + name_len + 1);
        i++;
    }
    return (NULL);
}