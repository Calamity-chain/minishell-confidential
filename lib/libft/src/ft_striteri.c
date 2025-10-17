/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:00:33 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:00:36 by ltoscani         ###   ########.fr       */
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
