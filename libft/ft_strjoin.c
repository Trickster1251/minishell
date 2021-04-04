/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 19:26:27 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/09 15:18:15 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	my_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char		*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	int		i;
	char	*a;

	if (!s1 || !s2)
		return (NULL);
	len = my_strlen(s1) + my_strlen(s2);
	a = (char*)malloc((len + 1) * sizeof(char));
	i = 0;
	if (a == NULL)
		return (NULL);
	while (s1[i])
	{
		a[i] = s1[i];
		i++;
	}
	while (*s2)
	{
		a[i] = *s2;
		s2++;
		i++;
	}
	a[i] = '\0';
	return (a);
}
