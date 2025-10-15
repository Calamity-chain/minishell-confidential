/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:10:46 by ltoscani          #+#    #+#             */
/*   Updated: 2025/06/06 13:11:03 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/**
 * @brief Validate if a string is a numeric argument
 */
static int	is_numeric(const char *str)
{
	int	i;
	int	has_digits;

	if (!str || !*str)
		return (0);
	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	has_digits = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		has_digits = 1;
		i++;
	}
	return (has_digits);
}

/**
 * @brief Built-in `exit`
 * @param args Arguments: args[0] is "exit", args[1] may be code.
 * @return Never returns (calls exit)
 */
int	ft_exit(char **args)
{
	int	exit_code;

	if (!args[1])
		exit(0);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1); // Don't exit
	}
	
	// Simple atoi with modulo 256 to match bash behavior
	exit_code = ft_atoi(args[1]) % 256;
	// Handle negative modulo to get positive result
	if (exit_code < 0)
		exit_code += 256;
	
	exit(exit_code);
}
