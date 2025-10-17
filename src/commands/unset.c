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

static int	remove_env_variable(t_data *data, char *var)
{
	int		i;
	int		j;
	int		count;
	char	**new_env;

	count = 0;
	while (data->env && data->env[count])
		count++;
	new_env = malloc(sizeof(char *) * count);
	if (!new_env)
		return (1);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (ft_strncmp(data->env[i], var, ft_strlen(var)) != 0 
			|| data->env[i][ft_strlen(var)] != '=')
			new_env[j++] = data->env[i];
		else
			free(data->env[i]);
		i++;
	}
	new_env[j] = NULL;
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
