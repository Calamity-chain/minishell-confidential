/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:53:58 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:54:02 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Adds a new node to the beginning of a singly-linked list */
void	ft_lstadd_front(t_list **head, t_list *new_node)
{
	new_node->next = *head;
}
