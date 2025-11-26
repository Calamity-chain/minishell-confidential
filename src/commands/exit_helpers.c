/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalniko <asalniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:48:41 by asalniko          #+#    #+#             */
/*   Updated: 2025/11/26 21:03:35 by asalniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <limits.h>

static void	print_exit_line(void)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
}

int	exit_too_many_args(t_data *data)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	data->exit_status = 1;
	return (1);
}

int	exit_no_args(t_data *data)
{
	print_exit_line();
	data->should_exit = 1;
	return (0);
}

int	exit_non_numeric(char *arg, t_data *data)
{
	print_exit_line();
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	data->exit_status = 2;
	data->should_exit = 1;
	return (2);
}

int	exit_with_code(char *arg, t_data *data)
{
	long			code;
	unsigned char	u;
	char			*clean;

	print_exit_line();
	clean = remove_quotes_for_exit(arg);
	if (!clean)
	{
		data->exit_status = 1;
		data->should_exit = 1;
		return (1);
	}
	code = ft_atoi(clean);
	free(clean);
	code %= 256;
	if (code < 0)
		code += 256;
	u = (unsigned char)code;
	data->exit_status = u;
	data->should_exit = 1;
	ft_free_matrix(data->env);
	data->env = NULL;
	return ((int)u);
}
