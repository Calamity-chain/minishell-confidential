/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:55:12 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:55:16 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Iterates a singly-linked list and applies 'f' on the content of each node.
 * Returns a new singly-linked list with the result, or NULL. */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*lst2;
	t_list	*content;
	t_list	*node;

	lst2 = NULL;
	while (lst)
	{
		content = f(lst->content);
		node = ft_lstnew(content);
		if (!node)
		{
			ft_lstclear(&lst2, del);
			break ;
		}
		ft_lstadd_back(&lst2, node);
		lst = lst->next;
	}
	return (lst2);
}
