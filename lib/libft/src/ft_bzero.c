/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 12:21:40 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:56:54 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Erases data by setting n bytes of data at memory location s to 0 */
void	ft_bzero(void *s, size_t n)
{
	while (n--)
		*((unsigned char *)s++) = '\0';
}
