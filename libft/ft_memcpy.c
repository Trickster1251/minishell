/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 16:24:43 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/08 17:12:28 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*a;
	const unsigned char	*b;

	a = dst;
	b = src;
	i = 0;
	if (a != b)
	{
		while (i < n)
		{
			*a = *b;
			a++;
			b++;
			i++;
		}
	}
	return (dst);
}
