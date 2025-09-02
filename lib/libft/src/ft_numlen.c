/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:38:09 by rmei              #+#    #+#             */
/*   Updated: 2024/08/22 15:43:09 by rmei             ###   ########.fr       */
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
