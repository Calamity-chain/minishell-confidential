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
	new_env[count + 1] = NULL;
	free(data->env);
	data->env = new_env;
	return (0);
}

int	ft_export(char **args, t_data *data)
{
	int	i;
	int	ret;

	if (!args[1])
	{
		i = 0;
		while (data->env[i])
			printf("declare -x %s\n", data->env[i++]);
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
		else if (add_env_variable(data, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
