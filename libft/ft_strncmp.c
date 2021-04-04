/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 22:38:16 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/01 23:22:54 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

int		ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*a;
	unsigned char	*b;
	size_t			i;

	a = (unsigned char*)s1;
	b = (unsigned char*)s2;
	i = 0;
	while ((*a || *b) && i < n)
	{
		if (*a != *b)
		{
			if (*a > *b)
				return (1);
			else if (*a < *b)
				return (-1);
			else
				return (0);
		}
		a++;
		b++;
		i++;
	}
	return (0);
}
