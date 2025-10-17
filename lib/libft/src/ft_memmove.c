/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:56:59 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:57:02 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Copies n bytes from memory area src to memory area dest. Memory can overlap.
 * Returns a pointer to dest. */
void	*ft_memmove(void *dest, const void *src, size_t n)
{
	const unsigned char	*ptr_src;
	unsigned char		*ptr_dest;

	if (!src && !dest)
		return (NULL);
	ptr_src = src;
	ptr_dest = dest;
	if (ptr_src < ptr_dest && ptr_src + n > ptr_dest)
		while (n--)
			ptr_dest[n] = ptr_src[n];
	else
		while (n--)
			*ptr_dest++ = *ptr_src++;
	return (dest);
}
