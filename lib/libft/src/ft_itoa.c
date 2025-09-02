/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 17:44:14 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 14:52:51 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Returns the length of a given number */
static int	ft_nlen(unsigned int n)
{
	int	n_len;

	if (!n)
		return (1);
	n_len = 0;
	while (n)
	{
		n_len++;
		n /= 10;
	}
	return (n_len);
}

/* Returns n to the power of m */
static int	ft_npow(int n, int m)
{
	if (!m)
		return (1);
	return (n * ft_npow(n, --m));
}

/* Returns a string representing n, or NULL if memory allocation fails */
char	*ft_itoa(int n)
{
	int				i;
	unsigned int	u_n;
	int				u_n_len;
	int				is_negative;
	char			*strn;

	u_n = n;
	is_negative = 0;
	if (n < 0)
	{
		u_n = -u_n;
		is_negative = 1;
	}
	u_n_len = ft_nlen(u_n);
	strn = (char *) malloc((u_n_len + is_negative + 1) * sizeof(char));
	if (!strn)
		return (NULL);
	i = 0;
	if (n < 0)
		strn[i++] = '-';
	u_n_len--;
	while (u_n_len >= 0)
		strn[i++] = (u_n / ft_npow(10, u_n_len--)) % 10 + '0';
	strn[i] = '\0';
	return (strn);
}
