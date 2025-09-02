/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 13:42:44 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:56:15 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Creates a substring from a given string, copying from to start up to len.
 * Returns a pointer to the new string, or NULL if the allocation fails. */
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	buffer_size;
	char	*substring;

	s_len = ft_strlen(s);
	if (start > s_len)
	{
		start = 0;
		len = 0;
	}
	else if (len + start > s_len)
		len = s_len - start;
	buffer_size = len + 1;
	substring = (char *) malloc(buffer_size * sizeof(char));
	if (!substring)
		return (NULL);
	ft_strlcpy(substring, s + start, buffer_size);
	return (substring);
}
