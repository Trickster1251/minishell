/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/07 14:55:51 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/16 11:17:45 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	words_count(char const *s, char c)
{
	int words_num;
	int k;

	words_num = 0;
	k = 0;
	while (*s)
	{
		if (*s != c && k == 0)
		{
			k = 1;
			words_num++;
		}
		if (*s == c && k == 1)
			k = 0;
		s++;
	}
	return (words_num);
}

static char	*my_strdup(const char *s1, char c)
{
	int		len;
	char	*s2;
	int		i;

	len = 0;
	i = 0;
	while (s1[len] != c && s1[len])
		len++;
	s2 = (char*)malloc((len + 1) * sizeof(char));
	if (s2 == NULL)
		return (NULL);
	while (s1[i] && s1[i] != c)
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}

static char	**w_free(char **words, int n)
{
	int i;

	i = 0;
	while (i < n)
	{
		free(words[i]);
		i++;
	}
	free(words);
	return (NULL);
}

static char	**w_malloc(const char *s, char c, char **words)
{
	int i;

	i = 0;
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			words[i] = my_strdup(s, c);
			if (words[i] == NULL)
				return (w_free(words, words_count(s, c)));
			i++;
			while (*s && *s != c)
				s++;
		}
	}
	words[i] = NULL;
	return (words);
}

char		**ft_split(char const *s, char c)
{
	char	**words;
	int		w_num;

	if (!s)
		return (NULL);
	w_num = words_count(s, c);
	words = (char**)malloc((w_num + 1) * sizeof(char*));
	if (words == NULL)
		return (NULL);
	words = w_malloc(s, c, words);
	return (words);
}
