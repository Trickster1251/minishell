/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 18:55:46 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/13 21:16:12 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	ft_strlen(char const *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char		*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*s1;
	size_t	i;
	size_t	k;

	if (!s)
		return (NULL);
	s1 = (char*)malloc((len + 1) * sizeof(char));
	if (s1 == NULL)
		return (NULL);
	if (ft_strlen(s) < (int)start)
	{
		s1[0] = 0;
		return (s1);
	}
	i = (size_t)start;
	len = len + i;
	k = 0;
	while (i < len && s[i])
	{
		s1[k] = s[i];
		i++;
		k++;
	}
	s1[k] = '\0';
	return (s1);
}
