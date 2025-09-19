/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:55:39 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:55:43 by ltoscani         ###   ########.fr       */
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
