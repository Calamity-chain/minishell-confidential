/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:01:23 by rmei              #+#    #+#             */
/*   Updated: 2024/08/22 15:42:32 by rmei             ###   ########.fr       */
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
