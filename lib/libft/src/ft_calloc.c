/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:20:21 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:52:09 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Allocates memory (set to 0) for a group of *n members* of *size* bytes.
 * Returns a ptr to the allocated memory. If the memory required would cause
 * overflow, nmemb or size is 0, or memory allocation fails, returns NULL */
void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	size_max;
	size_t	mem_req;
	void	*ptr;

	size_max = (size_t)(-1);
	if (size && nmemb > size_max / size)
		return (NULL);
	mem_req = nmemb * size;
	ptr = malloc(mem_req);
	if (!ptr)
		return (NULL);
	return (ft_memset(ptr, 0, mem_req));
}
