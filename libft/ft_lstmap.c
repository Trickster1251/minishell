/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 19:43:13 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/16 11:50:36 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list *new_lst;
	t_list *new_el;

	if (!lst || !f)
		return (NULL);
	new_lst = ft_lstnew((*f)((void*)lst->content));
	if (new_lst == NULL)
	{
		ft_lstclear(&lst, del);
		return (NULL);
	}
	lst = lst->next;
	while (lst != NULL)
	{
		new_el = ft_lstnew((*f)((void*)lst->content));
		if (new_el == NULL)
		{
			ft_lstclear(&new_lst, del);
			ft_lstclear(&lst, del);
			break ;
		}
		ft_lstadd_back(&new_lst, new_el);
		lst = lst->next;
	}
	return (new_lst);
}
