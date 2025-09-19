/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:02:24 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:02:27 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Locates the first occurrence of little within big, where only len characters
 * are searched. If '\0' is ecountered, it quits the search. Returns big in case
 * little is an empty string. If little is found, returns a pointer to the first
 * mathcing character in big, else returns NULL. */
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	unsigned int	i;
	unsigned int	j;

	if (!*little)
		return ((char *)big);
	i = 0;
	while (big[i] && len)
	{
		j = 0;
		while (big[i + j] == little[j] && j < len)
		{
			j++;
			if (!little[j])
				return ((char *)(&big[i]));
		}
		i++;
		len--;
	}
	return (NULL);
}
