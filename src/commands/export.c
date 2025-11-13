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

static int	print_env_export(t_data *data)
{
	int		i;
	char	*eq;

	i = 0;
	while (data->env && data->env[i])
	{
		eq = ft_strchr(data->env[i], '=');
		if (eq)
		{
			*eq = '\0';
			printf("declare -x %s=\"%s\"\n", data->env[i], eq + 1);
			*eq = '=';
		}
		else
			printf("declare -x %s\n", data->env[i]);
		i++;
	}
	return (0);
}

static int	export_one_assignment(t_data *data, char *arg)
{
	char	*eq;
	int		ret;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	*eq = '\0';
	ret = update_env_variable(data, arg, eq + 1);
	*eq = '=';
	return (ret);
}

static int	process_export_arg(t_data *data, char *arg)
{
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	if (ft_strchr(arg, '=') && export_one_assignment(data, arg) != 0)
		return (1);
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
		if (process_export_arg(data, args[i]) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
