/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:57:35 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:57:38 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Count the lenght of a number. */
size_t	ft_numlen(long long n)
{
	unsigned long long	un;
	size_t				len;

	un = n;
	len = 0;
	if (n <= 0)
	{
		un = -un;
		len++;
	}
	while (un > 0)
	{
		un /= 10;
		len++;
	}
	return (len);
}
