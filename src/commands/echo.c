/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:01:59 by ltoscani          #+#    #+#             */
/*   Updated: 2025/06/06 13:07:51 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

/**
 * @brief Built-in `echo` command with -n flag.
 * @param args Arguments including "echo" itself. args[0] == "echo"
 * @return 0 always (success)
 */
int	ft_echo(char **args)
{
	int	i;
	int	no_newline;
	int first_arg;

	i = 1;
	no_newline = 0;

	// Handle -n flag (can have multiple -n flags)
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		// Check if the entire argument is just "-n" or "-nnn..." 
		int j = 2;
		while (args[i][j] == 'n')
			j++;
		// If we reached the end of string, it's a valid -n flag
		if (args[i][j] != '\0')
			break;
		no_newline = 1;
		i++;
	}
	
	// Print all remaining arguments
	first_arg = 1;
	while (args[i])
	{
		if (!first_arg)
			write(STDOUT_FILENO, " ", 1);
		first_arg = 0;
		ft_putstr_fd(args[i], STDOUT_FILENO);
		i++;
	}
	// Add newline unless -n flag was used
	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
