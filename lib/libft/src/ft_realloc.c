/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 14:57:37 by rmei              #+#    #+#             */
/*   Updated: 2024/06/14 15:07:17 by rmei             ###   ########.fr       */
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
