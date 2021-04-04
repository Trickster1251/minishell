/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 20:31:35 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/13 21:21:44 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

char	*ft_strrchr(const char *s, int c)
{
	char	*s1;
	size_t	i;

	i = 0;
	s1 = (char *)s;
	while (s1[i])
		i++;
	if (c == 0)
		return (s1 + i);
	while (i--)
	{
		if (s1[i] == c)
			return (s1 + i);
	}
	return (NULL);
}
