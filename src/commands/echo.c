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

static int	is_nflag(const char *s)
{
	int	j;

	if (!s || s[0] != '-' || s[1] != 'n')
		return (0);
	j = 2;
	while (s[j] == 'n')
		j++;
	return (s[j] == '\0');
}

int	ft_echo(char **args)
{
	int	i;
	int	no_newline;
	int	first_arg;

	i = 1;
	no_newline = 0;
	while (args[i] && is_nflag(args[i]))
	{
		no_newline = 1;
		i++;
	}
	first_arg = 1;
	while (args[i])
	{
		if (!first_arg)
			write(STDOUT_FILENO, " ", 1);
		first_arg = 0;
		ft_putstr_fd(args[i], STDOUT_FILENO);
		i++;
	}
	if (!no_newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}
