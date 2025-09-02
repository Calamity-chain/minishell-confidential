/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 12:16:06 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:55:14 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Duplicates string s and returns a pointer to the new string (NULL on fail) */
char	*ft_strdup(const char *s)
{
	int		i;
	int		s_len;
	char	*dup;

	s_len = ft_strlen(s);
	dup = (char *) malloc((s_len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < s_len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
