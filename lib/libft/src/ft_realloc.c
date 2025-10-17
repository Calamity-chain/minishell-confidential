/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:59:34 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:59:39 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Reallocates memory to 'size', excess memory is not initialized */
void	*ft_realloc(void *ptr, size_t size)
{
	size_t			i;
	unsigned char	*uptr;
	unsigned char	*ptr_new;

	ptr_new = malloc(size);
	if (!ptr_new)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (ptr_new);
	i = 0;
	uptr = (unsigned char *)ptr;
	while (uptr[i] && i < size - 1)
	{
		ptr_new[i] = uptr[i];
		i++;
	}
	ptr_new[i] = '\0';
	free(ptr);
	return (ptr_new);
}
