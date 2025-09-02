/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 13:31:30 by rmei              #+#    #+#             */
/*   Updated: 2024/08/24 13:36:46 by rmei             ###   ########.fr       */
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
