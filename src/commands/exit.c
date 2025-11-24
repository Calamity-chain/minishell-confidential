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

static char	*strip_quotes(const char *s)
{
	size_t	i;
	size_t	j;
	char	*out;

	if (!s)
		return (NULL);
	out = malloc(ft_strlen(s) + 1);
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != '\'' && s[i] != '"')
			out[j++] = s[i];
		i++;
	}
	out[j] = '\0';
	return (out);
}

static int	is_numeric(const char *str)
{
	int		i;
	int		has_digits;
	char	*clean;

	if (!str || !*str)
		return (0);
	clean = strip_quotes(str);
	if (!clean)
		return (0);
	i = 0;
	if (clean[i] == '-' || clean[i] == '+')
		i++;
	has_digits = 0;
	while (clean[i])
	{
		if (!ft_isdigit((unsigned char)clean[i]))
			return (free(clean), 0);
		has_digits = 1;
		i++;
	}
	return (free(clean), has_digits);
}

char	*remove_quotes_for_exit(const char *str)
{
	return (strip_quotes(str));
}

int	ft_exit(char **args, t_data *data)
{
	if (args[1] && args[2])
		return (exit_too_many_args(data));
	if (!args[1])
		return (exit_no_args(data));
	if (!is_numeric(args[1]))
		return (exit_non_numeric(args[1], data));
	return (exit_with_code(args[1], data));
}
