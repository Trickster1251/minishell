/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/12 18:36:58 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/13 21:19:05 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list *a;

	a = lst;
	if (lst != NULL || f != NULL)
	{
		while (a != NULL)
		{
			(*f)((void*)a->content);
			a = a->next;
		}
	}
}
