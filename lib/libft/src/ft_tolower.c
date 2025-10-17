/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:03:46 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 15:03:51 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Converts a character to lowercase (ASCII standard set), if possible */
int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}
