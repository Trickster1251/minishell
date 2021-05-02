#include "includes/minishell.h"

int	ft_isdigit_str(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (0);
	return (1);
}

int	len_arr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	is_delim(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] == '=')
			return (i);
	}
	return (0);
}

void	print_arr(char **arr)
{
	int	i;
	int	j;

	i = -1;
	while (arr[++i])
	{
		j = -1;
		while (arr[i][++j])
			write(1, &arr[i][j], 1);
		write(1, "\n", 1);
	}
}
