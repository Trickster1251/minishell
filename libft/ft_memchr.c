/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:43:46 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/09 13:02:56 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*sc;
	size_t			i;

	sc = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if ((unsigned char)c == *sc)
			return ((void *)sc);
		sc++;
		i++;
	}
	return (NULL);
}
