/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/03 14:54:49 by rmei              #+#    #+#             */
/*   Updated: 2024/05/03 15:03:23 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Applies 'f' to each char of 's'. 'f' takes the index and the address of the
 * character as arguments. */
void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (*s)
		(*f)(i++, s++);
}
