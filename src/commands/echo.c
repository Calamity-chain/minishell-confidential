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
	no_newline = 1;

	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		no_newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (no_newline)
		printf("\n");
	return (0);
}

