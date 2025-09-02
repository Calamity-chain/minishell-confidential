/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:02:20 by rmei              #+#    #+#             */
/*   Updated: 2024/04/23 16:38:11 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Converts a character to lowercase (ASCII standard set), if possible */
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
