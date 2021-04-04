/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 16:30:16 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/13 21:29:33 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list *a;

	if (new == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		a = *lst;
		while (a->next != 0)
		{
			a = a->next;
		}
		a->next = new;
	}
}
