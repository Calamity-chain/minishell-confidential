/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 16:40:20 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:54:59 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Counts how many strings will result from the split of s by the delimeter c */
static int	ft_substr_count(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

/* Counts the length of the substring at 's' until delimeter 'c' is met */
static int	ft_substr_len(char const *s, char c)
{
	int	len;

	len = 0;
	while (*s && *(s++) != c)
		len++;
	return (len);
}

/* If a nested allocation fails, frees memory allocated to an array of ptrs */
static void	*ft_ptr_tab_free(char **ptr_tab, char **tab)
{
	while (ptr_tab != tab)
	{
		free(*ptr_tab);
		ptr_tab--;
	}
	free(*ptr_tab);
	free(ptr_tab);
	return (NULL);
}

/* Returns an array of strings, splitting 's' by the delimeter 'c', or NULL */
char	**ft_split(char const *s, char c)
{
	char	**tab;
	char	**ptr_tab;
	int		substr_len;

	tab = (char **) malloc((ft_substr_count(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (NULL);
	ptr_tab = tab;
	while (*s)
	{
		while (*s == c)
			s++;
		substr_len = ft_substr_len(s, c);
		if (!substr_len)
			break ;
		*ptr_tab = (char *) malloc((substr_len + 1) * sizeof(char));
		if (!*ptr_tab)
			return (ft_ptr_tab_free(ptr_tab, tab));
		ft_strlcpy(*ptr_tab, s, substr_len + 1);
		ptr_tab++;
		s += substr_len;
	}
	*ptr_tab = (void *)0;
	return (tab);
}
