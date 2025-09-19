/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:53:01 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:53:04 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Checks if a character is printable (ASCII standard set) */
int	ft_isprint(int c)
{
	return (c >= ' ' && c <= '~');
}
