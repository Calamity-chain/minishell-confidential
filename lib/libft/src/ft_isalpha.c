/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:52:25 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:52:29 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Checks if a character is alphabetic (ASCII standard set) */
int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
