/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 12:40:01 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:56:09 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Shifts the pointer to string 's' so the remove the 'set' characters from the
 * beginning of the string. Returns the updated pointer. */
static char const	*ft_strstarttrim(char const *s, char const *set)
{
	size_t	i;

	i = 0;
	while (*s && set[i])
	{
		if (*s == set[i++])
		{
			s++;
			i = 0;
			continue ;
		}
	}
	return (s);
}

/* Returns the length of string 's' so that the 'set' characters are removed */
static size_t	ft_strendfind(char const *s, int s_len, char const *set)
{
	size_t	i;

	i = 0;
	while (s_len && set[i])
	{
		while (s[s_len - 1] == set[i++])
		{
			s_len--;
			i = 0;
			continue ;
		}
	}
	return (s_len);
}

/* Returns a copy of 's1', removing the characters specified in 'set' from the
 * beginning and the end of 's1'. Returns NULL on failed memory allocation. */
char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	s1_len;
	size_t	buffer_size;
	char	*s2;

	if (!*s1)
		s1_len = 0;
	else if (!*set)
		s1_len = ft_strlen(s1);
	else
	{
		s1 = ft_strstarttrim(s1, set);
		s1_len = ft_strendfind(s1, ft_strlen(s1), set);
	}
	buffer_size = s1_len + 1;
	s2 = (char *) malloc(buffer_size * sizeof(char));
	if (!s2)
		return (NULL);
	ft_strlcpy(s2, s1, buffer_size);
	return (s2);
}
