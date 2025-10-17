/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:52:37 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:52:41 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Checks if a character exists in the standard ASCII set */
int	ft_isascii(int c)
{
	return (c >= 0 && c <= 127);
}
