#include "includes/minishell.h"

char		*ft_charjoin(char const *s1, char c)
{
	int		len;
	int		i;
	char	*a;

	if (!s1)
		return (NULL);
	len = ft_strlen((char *)s1) + 1;
	a = (char*)malloc((len + 1) * sizeof(char));
	i = 0;
	if (a == NULL)
		return (NULL);
	while (s1[i])
	{
		a[i] = s1[i];
		i++;
	}
	a[i] = c;
	a[i + 1] = '\0';
	free ((char *)s1);
	return (a);
}

char	**realoc_argv(char **src)
{
	char **new;
	int len;
	int i;

	i = 0;
	len = argv_len(src);
	new = (char**)malloc(sizeof(char*) * (len + 2));
	char *s;
	while (i < len)
	{
		s = src[i];
		new[i] = src[i];
		i++;
	}
	new[len + 1] = NULL;
	free(src);
	return(new);
}

char	**arg_join(t_all *all, char **argv, char *str)
{
	char **tmp;
	int len;
	char *s;

	if (argv == NULL)
	{
		argv = (char**)ft_calloc(2, sizeof(char*));
		if (!argv)
			return (NULL);
		argv[0] = str;
		tmp = argv;
	}
	else
	{
		len = argv_len(argv);
		tmp = realoc_argv(argv);
		if (!tmp)
		{
			print_merror(all);
			return (NULL);
		}
		tmp[len] = str;
	}
	return (tmp);
}