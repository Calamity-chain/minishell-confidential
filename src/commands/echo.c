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

	i = 1;
	no_newline = 0;

	// Handle -n flag (can have multiple -n flags)
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		// Check if the entire argument is just "-n" or "-nnn..." 
		int j = 1;
		while (args[i][j] == 'n')
			j++;
		// If we reached the end of string, it's a valid -n flag
		if (args[i][j] == '\0')
		{
			no_newline = 1;
			i++;
		}
		else
		{
			break; // Not a pure -n flag, treat as regular argument
		}
	}
	
	// Print all remaining arguments
	int	first_arg = 1;
	while (args[i])
	{
		if (!first_arg)
			write(STDOUT_FILENO, " ", 1);
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		first_arg = 0;
		i++;
	}
	
	// Add newline unless -n flag was used
	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);
	
	return (0);
}
