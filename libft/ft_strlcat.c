/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 20:16:18 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/13 21:14:00 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static size_t	ft_strlen(const char *src)
{
	size_t	i;

	i = 0;
	while (*src)
	{
		i++;
		src++;
	}
	return (i);
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t			i;
	size_t			k;

	i = 0;
	k = 0;
	while (dst[i] && i < dstsize)
		i++;
	if (dstsize == 0)
		return (ft_strlen(src) + i);
	while (((k + i) < dstsize - 1) && src[k])
	{
		dst[k + i] = src[k];
		k++;
	}
	if (i < dstsize)
		dst[k + i] = '\0';
	return (ft_strlen(src) + i);
}
