/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:03:23 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:03:26 by ltoscani         ###   ########.fr       */
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
