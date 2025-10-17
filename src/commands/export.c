/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:45:41 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/14 19:45:46 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !*str || ft_isdigit(*str) || *str == '=')
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	add_env_variable(t_data *data, char *var)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while (data->env && data->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = -1;
	while (++i < count)
		new_env[i] = data->env[i];
	new_env[count] = ft_strdup(var);
	if (!new_env[count])
	{
		free(new_env);
		return (1);
	}
	new_env[count + 1] = NULL;
	free(data->env);
	data->env = new_env;
	return (0);
}

static int	add_or_update_env_variable(t_data *data, char *var)
{
	int		i;
	size_t	var_len;
	char	*equals_pos;

	// Find the variable name part (before =)
	equals_pos = ft_strchr(var, '=');
	if (!equals_pos)
		return (0);  // No value to set
		
	var_len = equals_pos - var;
	
	// Check if variable already exists
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, var_len) == 0 && data->env[i][var_len] == '=')
		{
			// Update existing variable
			free(data->env[i]);
			data->env[i] = ft_strdup(var);
			return (data->env[i] ? 0 : 1);
		}
		i++;
	}
	
	// Variable doesn't exist, add it
	return (add_env_variable(data, var));
}

int	ft_export(char **args, t_data *data)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		// Print environment variables
		i = 0;
		while (data->env[i])
		{
			// Format as "declare -x VAR=value"
			char *equals = ft_strchr(data->env[i], '=');
			if (equals)
			{
				*equals = '\0';
				printf("declare -x %s=\"%s\"\n", data->env[i], equals + 1);
				*equals = '=';
			}
			else
			{
				printf("declare -x %s\n", data->env[i]);
			}
			i++;
		}
		return (0);
	}
	
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			ret = 1;
		}
		else if (ft_strchr(args[i], '='))
		{
			if (add_or_update_env_variable(data, args[i]) != 0)
				ret = 1;
		}
		// If no '=', just validate but don't add/update (bash behavior)
		i++;
	}
	return (ret);
}
