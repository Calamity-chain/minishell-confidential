/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:00:49 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:00:52 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Joins strings s1 and s2 into a new string and returns it, or NULL on fail */
char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	buffer_size;
	char	*s3;

	buffer_size = ft_strlen(s1) + ft_strlen(s2) + 1;
	s3 = (char *) malloc(buffer_size * sizeof(char));
	if (!s3)
		return (NULL);
	ft_strlcpy(s3, s1, buffer_size);
	ft_strlcat(s3, s2, buffer_size);
	return (s3);
}
