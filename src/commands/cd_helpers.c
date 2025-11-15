/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helpers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 16:56:32 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/15 16:56:34 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*make_pair(const char *key, const char *val)
{
	char	*tmp;
	char	*out;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	out = ft_strjoin(tmp, val);
	free(tmp);
	return (out);
}

int	add_env_variable(t_data *data, char *var, char *value)
{
	int		count;
	char	**new_env;
	char	*pair;

	count = 0;
	while (data->env && data->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	if (count > 0)
		ft_memcpy(new_env, data->env, sizeof(char *) * count);
	pair = make_pair(var, value);
	if (!pair)
		return (free(new_env), 1);
	new_env[count] = pair;
	new_env[count + 1] = NULL;
	free(data->env);
	data->env = new_env;
	return (0);
}

int	update_env_variable(t_data *data, char *var, char *value)
{
	int		i;
	size_t	vlen;
	char	*pair;

	if (!data || !data->env || !var || !value)
		return (1);
	vlen = ft_strlen(var);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, vlen) == 0
			&& data->env[i][vlen] == '=')
		{
			pair = make_pair(var, value);
			if (!pair)
				return (1);
			free(data->env[i]);
			data->env[i] = pair;
			return (0);
		}
		i++;
	}
	return (add_env_variable(data, var, value));
}

char	*get_home_path(t_data *data)
{
	char	*home;

	home = ft_getenv(data, "HOME");
	if (!home)
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	return (home);
}

char	*get_oldpwd_path(t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_getenv(data, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		return (NULL);
	}
	return (oldpwd);
}
