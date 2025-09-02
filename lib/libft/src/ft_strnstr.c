/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 10:18:59 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:56:01 by rmei             ###   ########.fr       */
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
