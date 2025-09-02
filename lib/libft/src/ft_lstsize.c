/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 12:55:33 by rmei              #+#    #+#             */
/*   Updated: 2024/05/05 13:03:52 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Counts the number of nodes in a singly-linked list */
int	ft_lstsize(t_list *lst)
{
	int	lsize;

	lsize = 0;
	while (lst)
	{
		lsize++;
		lst = lst->next;
	}
	return (lsize);
}
