#include "../includes/minishell.h"

void	init_tok(t_tok *tok, char *str)
{
	tok->res = NULL;
	tok->str_tmp = ft_strtrim(str, " ");
	tok->tmp = ft_strdup("");
}

int	is_space_arg(char **str)
{
	int	i;
	int	j;
	int	ret;

	i = 0;
	ret = 0;
	while (str[i])
	{
		j = 0;
		while (str[i][j])
		{
			if (str[i][j] != ' ')
				ret++;
			j++;
		}
		i++;
	}
	return (ret);
}

void	free_tok(t_tok *tok, char *str)
{
	free(tok->str_tmp);
	free(str);
}
