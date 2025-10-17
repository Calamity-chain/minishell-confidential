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
#include <limits.h>

/**
 * @brief Check if string is numeric, handling quotes and signs
 */
static int	is_numeric(const char *str)
{
	int	i;
	int	has_digits;
	char	*clean;
	int	j;

	if (!str || !*str)
		return (0);
	
	// Remove all quotes
	clean = malloc(ft_strlen(str) + 1);
	j = 0;
	for (i = 0; str[i]; i++)
	{
		if (str[i] != '"' && str[i] != '\'')
			clean[j++] = str[i];
	}
	clean[j] = '\0';
	
	// Check if clean string is numeric
	i = 0;
	if (clean[i] == '-' || clean[i] == '+')
		i++;
	
	has_digits = 0;
	while (clean[i])
	{
		if (!ft_isdigit(clean[i]))
		{
			free(clean);
			return (0);
		}
		has_digits = 1;
		i++;
	}
	
	free(clean);
	return (has_digits);
}

/**
 * @brief Remove quotes from a string for exit code calculation
 */
static char	*remove_quotes_for_exit(const char *str)
{
	int		i;
	int		j;
	char	*result;
	int		in_quotes;
	char	quote_char;

	if (!str)
		return (NULL);
	
	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	in_quotes = 0;
	quote_char = 0;
	
	while (str[i])
	{
		if (!in_quotes && (str[i] == '"' || str[i] == '\''))
		{
			in_quotes = 1;
			quote_char = str[i];
		}
		else if (in_quotes && str[i] == quote_char)
		{
			in_quotes = 0;
		}
		else
		{
			result[j++] = str[i];
		}
		i++;
	}
	result[j] = '\0';
	
	return (result);
}

/**
 * @brief Built-in `exit`
 */
int	ft_exit(char **args)
{
	long	exit_code;
	char	*clean_arg;
	
	// Check for too many arguments
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);  // Don't exit, just return error
	}
	if (!args[1])
		exit(0);
	
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", STDERR_FILENO);
		exit(2);
	}
	
	// Remove quotes before conversion
	clean_arg = remove_quotes_for_exit(args[1]);
	if (!clean_arg)
		exit(1);
	
	exit_code = ft_atoi(clean_arg);
	free(clean_arg);
	
	// Handle exit codes modulo 256
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	
	exit((unsigned char)exit_code);
}
