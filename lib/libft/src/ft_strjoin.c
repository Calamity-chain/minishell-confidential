/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 11:58:17 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:55:21 by rmei             ###   ########.fr       */
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
