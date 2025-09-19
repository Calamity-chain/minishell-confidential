/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:51:25 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:51:29 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Raise an error and quit the program. */
void	ft_error(char *msg)
{
	char	*error;

	error = ft_strjoin("[ERROR] - ", msg);
	ft_putendl_fd(error, STDERR_FILENO);
	free(error);
	exit(1);
}
