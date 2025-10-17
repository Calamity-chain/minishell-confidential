/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:50:46 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:50:50 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Erases data by setting n bytes of data at memory location s to 0 */
void	ft_bzero(void *s, size_t n)
{
	while (n--)
		*((unsigned char *)s++) = '\0';
}
