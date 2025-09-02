/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:38:20 by rmei              #+#    #+#             */
/*   Updated: 2024/04/26 18:38:33 by rmei             ###   ########.fr       */
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
