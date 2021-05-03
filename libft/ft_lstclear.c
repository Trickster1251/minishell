/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 17:33:59 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/16 13:48:20 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*a;

	if (lst != NULL)
	{
		while (*lst != NULL)
		{
			(*del)((void *)(*lst)->content);
			a = (*lst)->next;
			free(*lst);
			*lst = a;
		}
		*lst = NULL;
	}
}
