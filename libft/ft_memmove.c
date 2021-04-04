/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:37:12 by aemustaf          #+#    #+#             */
/*   Updated: 2020/10/31 21:25:26 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	i = 0;
	d = dst;
	s = src;
	if (d < s)
	{
		while (i < len)
		{
			*d++ = *s++;
			i++;
		}
	}
	else if (d > s)
	{
		s += len - 1;
		d += len - 1;
		while (len--)
		{
			*d-- = *s--;
		}
	}
	return (dst);
}
