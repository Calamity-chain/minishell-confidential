/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:53:42 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:53:49 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Adds a new node to the end of a singly-linked list */
void	ft_lstadd_back(t_list **head, t_list *new_node)
{
	t_list	*tail;

	if (!*head)
		*head = new_node;
	else
	{
		tail = ft_lstlast(*head);
		tail->next = new_node;
	}
}
