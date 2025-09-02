/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_haschar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 09:39:48 by rmei              #+#    #+#             */
/*   Updated: 2024/05/18 09:41:27 by rmei             ###   ########.fr       */
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
