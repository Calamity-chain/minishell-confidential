/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 13:59:46 by rmei              #+#    #+#             */
/*   Updated: 2024/07/17 18:29:47 by rmei             ###   ########.fr       */
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
