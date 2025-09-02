/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 15:40:33 by rmei              #+#    #+#             */
/*   Updated: 2024/05/18 10:35:21 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Prints integer 'n' to 'fd', returns n_chars printed */
int	ft_putnbr_fd(long n, int fd)
{
	int				n_chars;
	unsigned int	b_len;
	unsigned long	u_n;
	char			char_n;

	n_chars = 0;
	u_n = n;
	if (n < 0)
	{
		u_n = -n;
		n_chars += ft_putchar_fd('-', fd);
	}
	b_len = 10;
	if (u_n / b_len)
		n_chars += ft_putnbr_fd(u_n / b_len, fd);
	char_n = (u_n % b_len) + '0';
	ft_putchar_fd(char_n, fd);
	return (++n_chars);
}
