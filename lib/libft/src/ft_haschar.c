/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_haschar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:51:48 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:51:53 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Checks if a string contains a certain char */
int	ft_haschar(char *s, char c)
{
	while (*s)
		if (*(s++) == c)
			return (1);
	return (0);
}
