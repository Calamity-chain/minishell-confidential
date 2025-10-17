/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:03:58 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:04:01 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Converts a character to uppercase, if possible */
int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	return (c);
}
