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
	int		found;

	if (!data || !data->env || !var)
		return (0);  // Return 0 for "variable not found"
	
	// First, check if variable exists and count total variables
	count = 0;
	found = 0;
	while (data->env[count])
	{
		if (ft_strncmp(data->env[count], var, ft_strlen(var)) == 0 
			&& data->env[count][ft_strlen(var)] == '=')
			found = 1;
		count++;
	}
	
	// If variable not found, return success
	if (!found)
		return (0);
	
	// Allocate new environment (size will be count, since we remove one)
	new_env = malloc(sizeof(char *) * count);  // count, not count+1
	if (!new_env)
		return (1);
	
	// Copy all variables except the one to remove
	i = 0;
	j = 0;
	while (i < count)
	{
		if (ft_strncmp(data->env[i], var, ft_strlen(var)) != 0 
			|| data->env[i][ft_strlen(var)] != '=')
		{
			new_env[j] = data->env[i];  // Transfer ownership
			j++;
		}
		else
		{
			free(data->env[i]);  // Free the removed variable
		}
		i++;
	}
	new_env[j] = NULL;
	
	// Free the old array (but NOT the strings - they're now in new_env)
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
		// Only return error on actual failures, not "variable not found"
		if (remove_env_variable(data, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
