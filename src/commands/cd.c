/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 19:45:31 by ltoscani          #+#    #+#             */
/*   Updated: 2025/10/14 19:45:34 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// MOVE add_env_variable to the top, before update_env_variable
static int	add_env_variable(t_data *data, char *var, char *value)
{
	int		count;
	int		i;
	char	**new_env;
	char	*new_var;

	count = 0;
	while (data->env && data->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (1);
	i = -1;
	while (++i < count)
		new_env[i] = data->env[i];
	new_var = ft_strjoin(var, "=");
	if (!new_var)
	{
		free(new_env);
		return (1);
	}
	new_env[count] = ft_strjoin(new_var, value);
	free(new_var);
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

static int	update_env_variable(t_data *data, char *var, char *value)
{
	int		i;
	char	*new_env;
	size_t	var_len;

	if (!data || !data->env || !var || !value)
		return (1);
	var_len = ft_strlen(var);
	i = 0;
	while (data->env[i])
	{
		if (ft_strncmp(data->env[i], var, var_len) == 0 && data->env[i][var_len] == '=')
		{
			new_env = ft_strjoin(var, "=");
			if (!new_env)
				return (1);
			free(data->env[i]);
			data->env[i] = ft_strjoin(new_env, value);
			free(new_env);
			if (!data->env[i])
				return (1);
			return (0);
		}
		i++;
	}
	// If variable doesn't exist, add it
	return (add_env_variable(data, var, value));
}

static char	*get_home_path(t_data *data)
{
	char	*home;

	home = ft_getenv(data, "HOME");
	if (!home)
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	return (home);
}

static char	*get_oldpwd_path(t_data *data)
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

int	ft_cd(char **args, t_data *data)
{
	char	*path;
	char	cwd[1024];
	char	old_cwd[1024];

	// Get current directory BEFORE changing
	if (!getcwd(old_cwd, sizeof(old_cwd)))
		return (perror("cd"), 1);

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
	{
		path = get_home_path(data);
		if (!path)
			return (1);
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_oldpwd_path(data);
		if (!path)
			return (1);
		printf("%s\n", path);
	}
	else
		path = args[1];

	if (chdir(path) != 0)
	{
    		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
    		perror(path);  // This will print "path: Error message"
    		return (1);
	}
	// Update OLDPWD with the OLD directory (before cd)
	if (update_env_variable(data, "OLDPWD", old_cwd) != 0)
		return (1);

	// Update PWD with the NEW directory (after cd)
	if (getcwd(cwd, sizeof(cwd)))
	{
		if (update_env_variable(data, "PWD", cwd) != 0)
			return (1);
	}

	return (0);
}
