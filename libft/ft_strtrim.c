/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/04 20:16:22 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/09 17:44:02 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char		*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	char	*a;
	int		end;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = ft_strlen((char*)s1) - 1;
	while (end && ft_strchr(set, s1[end]))
		end--;
	if (s1[start] == 0)
		return (ft_calloc(1, sizeof(char)));
	a = ft_substr(s1, start, end - start + 1);
	return (a);
}
