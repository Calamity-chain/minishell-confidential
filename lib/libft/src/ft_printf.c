/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 11:28:04 by rmei              #+#    #+#             */
/*   Updated: 2024/08/21 16:04:41 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Prints a pointer address, returns the n_chars printed */
static int	ft_print_ptr(va_list args, char *hex_base)
{
	char	*ptr;

	ptr = va_arg(args, void *);
	if (!ptr)
		return (ft_putstr_fd("(nil)", 1));
	return (ft_putstr_fd("0x", 1) \
			+ ft_putnbr_base_fd((unsigned long)ptr, 1, hex_base));
}

/* Prints to screen the current variable, returns the n_chars printed */
static int	ft_print_arg(const char *fmt, va_list args)
{
	char	*hex_base;
	char	*hex_base_cap;

	hex_base = "0123456789abcdef";
	hex_base_cap = "0123456789ABCDEF";
	if (*fmt == 'c')
		return (ft_putchar_fd(va_arg(args, int), 1));
	else if (*fmt == 's')
		return (ft_putstr_fd(va_arg(args, char *), 1));
	else if (*fmt == 'p')
		return (ft_print_ptr(args, hex_base));
	else if (*fmt == 'd' || *fmt == 'i')
		return (ft_putnbr_fd(va_arg(args, int), 1));
	else if (*fmt == 'u')
		return (ft_putnbr_fd(va_arg(args, unsigned int), 1));
	else if (*fmt == 'x')
		return (ft_putnbr_base_fd(va_arg(args, unsigned int), 1, hex_base));
	else if (*fmt == 'X')
		return (ft_putnbr_base_fd(va_arg(args, unsigned int), 1, hex_base_cap));
	return (ft_putchar_fd(*fmt, 1));
}

/* Manage printing 'cspdiuxX%' specifiers. Doesn't handle buffer size.
 * Returns the number of chars printed. */
int	ft_printf(const char *fmt, ...)
{
	int		n_chars;
	va_list	args;

	if (!fmt)
		return (-1);
	n_chars = 0;
	va_start(args, fmt);
	while (*fmt)
	{
		while (*fmt && *fmt != '%')
		{
			ft_putchar_fd(*fmt, 1);
			n_chars++;
			fmt++;
		}
		if (!*fmt)
			break ;
		fmt++;
		n_chars += ft_print_arg(fmt, args);
		fmt++;
	}
	va_end(args);
	return (n_chars);
}
