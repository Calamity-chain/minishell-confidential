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

static int	cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	perror(path);
	return (1);
}

static int	check_cd_args(char **args)
{
	int	arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count == 2)
	{
		if (args[1]
			&& (args[1][0] == '/' || ft_strchr(args[1], '$')
			|| ft_strchr(args[1], '~')))
		{
			ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO);
			return (1);
		}
	}
	if (arg_count > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

static char	*get_target_path(char **args, t_data *data)
{
	char	*path;

	if (!args[1] || ft_strncmp(args[1], "~", 2) == 0)
	{
		path = get_home_path(data);
		if (!path)
			return (NULL);
		return (path);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_oldpwd_path(data);
		if (!path)
			return (NULL);
		printf("%s\n", path);
		return (path);
	}
	return (args[1]);
}

int	ft_cd(char **args, t_data *data)
{
	char	*path;
	char	cwd[1024];
	char	old_cwd[1024];

	if (check_cd_args(args) != 0)
		return (1);
	if (!getcwd(old_cwd, sizeof(old_cwd)))
		return (perror("cd"), 1);
	path = get_target_path(args, data);
	if (!path)
		return (1);
	if (chdir(path) != 0)
		return (cd_error(path));
	if (update_env_variable(data, "OLDPWD", old_cwd) != 0)
		return (1);
	if (getcwd(cwd, sizeof(cwd)))
	{
		if (update_env_variable(data, "PWD", cwd) != 0)
			return (1);
	}
	return (0);
}
