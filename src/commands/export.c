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
	char	**new_env;

	count = 0;
	while (data->env && data->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	if (count > 0)
		ft_memcpy(new_env, data->env, sizeof(char *) * count);
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
	char	*equal;

	equal = ft_strchr(var, '=');
	if (!equal)
		return (0);
	var_len = (size_t)(equal - var);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, var_len) == 0
			&& data->env[i][var_len] == '=')
		{
			free(data->env[i]);
			data->env[i] = ft_strdup(var);
			if (data->env[i])
				return (0);
			return (1);
		}
		i++;
	}
	return (add_env_variable(data, var));
}

static int	print_env_export(t_data *data)
{
	int		i;
	char	*equal;

	i = 0;
	while (data->env && data->env[i])
	{
		equal = ft_strchr(data->env[i], '=');
		if (equal)
		{
			*equal = '\0';
			printf("declare -x %s=\"%s\"\n", data->env[i], equal + 1);
			*equal = '=';
		}
		else
			printf("declare -x %s\n", data->env[i]);
		i++;
	}
	return (0);
}

int	ft_export(char **args, t_data *data)
{
	int	i;
	int	ret;

	if (!args || !data)
		return (1);
	if (!args[1])
		return (print_env_export(data));
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
		else if (ft_strchr(args[i], '=') != NULL
			&& add_or_update_env_variable(data, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
