/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:02:48 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:02:51 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Returns a pointer to the last occurence of c in s, or NULL. If c is '\0',
 * it returns a pointer to the null terminator. */
char	*ft_strrchr(const char *s, int c)
{
	int		len_s;
	char	*ptr_s;

	len_s = ft_strlen(s);
	ptr_s = (char *)s + len_s;
	if ((char)c == '\0')
		return (ptr_s);
	while (len_s-- >= 0)
	{
		if (*ptr_s == (char)c)
			return (ptr_s);
		ptr_s--;
	}
	return (NULL);
}
