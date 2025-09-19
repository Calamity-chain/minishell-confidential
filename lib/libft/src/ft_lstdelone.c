/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:54:25 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:54:30 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* Frees the memory on a node's content using the function given as argument,
 * then frees the memory of the node. It does not free the memory of 'next'. */
void	ft_lstdelone(t_list *node, void (*del)(void *))
{
	(*del)(node->content);
	free(node);
}
