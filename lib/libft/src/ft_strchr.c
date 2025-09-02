/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:28:18 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:55:09 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Returns a pointer to the first occurence of c in s, or NULL. If c is '\0',
 * it returns a pointer to the null terminator. */
char	*ft_strchr(const char *s, int c)
{
	char	*ptr_s;

	ptr_s = (char *)s;
	while (*ptr_s)
	{
		if (*ptr_s == (char)c)
			return (ptr_s);
		ptr_s++;
	}
	if ((char)c == '\0')
		return (ptr_s);
	return (NULL);
}
