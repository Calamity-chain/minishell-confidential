/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:00:05 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:00:09 by ltoscani         ###   ########.fr       */
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
