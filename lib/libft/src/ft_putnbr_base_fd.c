/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 12:08:16 by rmei              #+#    #+#             */
/*   Updated: 2024/05/18 10:35:57 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_base_valid(char *base)
{
	int	i;
	int	j;

	if (!*base || !base[1])
		return (0);
	i = 0;
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-'
			|| (base[i] < '!' && base[i] > '~'))
			return (0);
		j = 0;
		while (base[j])
		{
			if (j != i && base[j] == base[i])
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

/* Writes 'n' to 'fd' in a certain base, returns the n_chars printed */
int	ft_putnbr_base_fd(long nbr, int fd, char *base)
{
	int				n_chars;
	unsigned int	b_len;
	unsigned long	n;

	if (!ft_is_base_valid(base))
	{
		ft_putstr_fd("[BASE ERROR] Invalid base.", 2);
		return (0);
	}
	n = nbr;
	n_chars = 0;
	b_len = ft_strlen(base);
	if (nbr < 0 && ft_strncmp(base, "0123456789", b_len) == 0)
	{
		n = -nbr;
		n_chars += ft_putchar_fd('-', fd);
	}
	if (n / b_len)
		n_chars += ft_putnbr_base_fd((n / b_len), fd, base);
	ft_putchar_fd(base[n % b_len], 1);
	return (++n_chars);
}
