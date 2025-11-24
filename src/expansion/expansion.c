/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:50:23 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/24 21:51:05 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/lexer.h"
#include "../include/parser.h"

void	handle_env(t_data *data, t_command *command, int *ac, t_token *token)
{
	char	*val;

	(void)command;
	(void)ac;
	if (!token || !token->value)
		return ;
	val = expand_env_variable(data, token->value);
	if (!val)
		return ;
	free(token->value);
	token->value = val;
}

char	*expand_tilda(t_data *data, const char *env_name)
{
	const char	*home;
	const char	*suf;
	char		*res;
	size_t		hl;
	size_t		sl;

	if (!env_name)
		return (NULL);
	if (env_name[1] != '/' && env_name[1] != '\0')
		return (ft_strdup(env_name));
	home = ft_getenv(data, "HOME");
	if (!home)
		return (ft_strdup(env_name));
	suf = env_name + 1;
	hl = ft_strlen(home);
	sl = ft_strlen(suf);
	res = (char *)malloc(hl + sl + 1);
	if (!res)
		return (NULL);
	ft_memcpy(res, home, hl);
	ft_memcpy(res + hl, suf, sl);
	res[hl + sl] = '\0';
	return (res);
}

char	*expand_env_variable(t_data *data, const char *env_name)
{
	const char	*val;

	if (!data || !env_name || env_name[0] == '\0')
		return (ft_strdup(""));
	if (ft_strncmp(env_name, "$?", 3) == 0)
		return (ft_itoa(data->exit_status));
	if (env_name[0] == '~')
		return (expand_tilda(data, env_name));
	val = ft_getenv(data, env_name);
	if (!val)
		return (ft_strdup(""));
	return (ft_strdup(val));
}

char	*ft_getenv(t_data *data, const char *env_name)
{
	size_t		nlen;
	const char	*name;
	int			i;

	if (!data || !data->env || !env_name)
		return (NULL);
	name = env_name;
	if (*name == '$')
		name++;
	nlen = ft_strlen(name);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], name, nlen) == 0
			&& data->env[i][nlen] == '=')
			return (data->env[i] + nlen + 1);
		i++;
	}
	return (NULL);
}
