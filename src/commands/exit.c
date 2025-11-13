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

static char	*remove_quotes_for_exit(const char *str)
{
	return (strip_quotes(str));
}

static void	print_exit_line(void)
{
	ft_putendl_fd("exit", STDOUT_FILENO);
}

int	ft_exit(char **args)
{
	long	exit_code;
	char	*clean_arg;

	if (args[1] && args[2])
		return (ft_putstr_fd("minishell: exit: too many arguments\n", 2), 1);
	if (!args[1])
		return (print_exit_line(), exit(0), 0);
	if (!is_numeric(args[1]))
	{
		print_exit_line();
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	clean_arg = remove_quotes_for_exit(args[1]);
	if (!clean_arg)
		return (print_exit_line(), exit(1), 0);
	exit_code = ft_atoi(clean_arg);
	free(clean_arg);
	exit_code %= 256;
	if (exit_code < 0)
		exit_code += 256;
	print_exit_line();
	exit((unsigned char)exit_code);
}
