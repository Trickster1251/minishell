/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 20:31:00 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/20 20:31:02 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		ft_strlen(const char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*my_strjoin(char *s1, char *s2)
{
	int		len;
	int		i;
	char	*a;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	a = (char*)malloc((len + 1) * sizeof(char));
	i = 0;
	if (a == NULL)
		return (NULL);
	while (s1[i])
	{
		a[i] = s1[i];
		i++;
	}
	free(s1);
	while (*s2)
	{
		a[i] = *s2;
		s2++;
		i++;
	}
	a[i] = '\0';
	return (a);
}

char	*ft_strdup(const char *s1)
{
	int		len;
	char	*s2;
	int		i;

	len = 0;
	i = 0;
	while (s1[len])
		len++;
	s2 = (char*)malloc((len + 1) * sizeof(char));
	if (s2 == NULL)
		return (NULL);
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

char	*ft_strchr(const char *s, int c)
{
	char *s1;

	s1 = (char*)s;
	if (c == 0)
	{
		while (*s1)
			s1++;
		return (s1);
	}
	while (*s1)
	{
		if (*s1 == c)
			return (s1);
		s1++;
	}
	return (NULL);
}
