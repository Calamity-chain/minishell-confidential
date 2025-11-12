/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:45:54 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/14 19:45:56 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

static int	count_env_and_found(t_data *data, const char *var,
				size_t len, int *found)
{
	int	i;

	*found = 0;
	i = 0;
	while (data->env[i])
	{
		if (!ft_strncmp(data->env[i], var, len)
			&& data->env[i][len] == '=')
			*found = 1;
		i++;
	}
	return (i);
}

static int	copy_env_without_key(t_data *data, const char *var,
				size_t len, char **dst)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (data->env[i])
	{
		if (!(!ft_strncmp(data->env[i], var, len)
				&& data->env[i][len] == '='))
			dst[j++] = data->env[i];
		else
			free(data->env[i]);
		i++;
	}
	dst[j] = NULL;
	return (j);
}

static int	remove_env_variable(t_data *data, char *var)
{
	int		n;
	int		found;
	size_t	len;
	char	**new_env;

	if (!data || !data->env || !var)
		return (0);
	len = ft_strlen(var);
	n = count_env_and_found(data, var, len, &found);
	if (!found)
		return (0);
	new_env = malloc(sizeof(char *) * (n + 1));
	if (!new_env)
		return (1);
	copy_env_without_key(data, var, len, new_env);
	free(data->env);
	data->env = new_env;
	return (0);
}

int	ft_unset(char **args, t_data *data)
{
	int	i;
	int	ret;

	if (!args[1])
		return (0);
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (remove_env_variable(data, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
