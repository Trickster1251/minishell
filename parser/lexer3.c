#include "../includes/minishell.h"

int	remove_ch(t_line *src)
{
	char	*tmp;

	tmp = ft_strdup("");
	if (!tmp)
		return (-1);
	src->pos = 0;
	while (src->str[src->pos])
	{
		if (src->str[src->pos] != '\\' && src->str[src->pos] != '"'
			&& src->str[src->pos] != '\'' && src->str[src->pos] != '\n')
		{
			tmp = ft_charjoin(tmp, src->str[src->pos]);
			if (!tmp)
				return (-1);
		}
		src->pos++;
	}
	free(src->str);
	src->str = tmp;
	return (0);
}

int	if_first_redir(char **argv)
{
	int	i;
	int	fd;

	i = argv_len(argv);
	if ((!ft_strncmp(argv[0], ">", 2) || !ft_strncmp(argv[0], ">>", 3))
		&& i == 2)
	{
		fd = open(argv[1], O_CREAT | O_RDWR, 0777);
		if (fd < 0)
		{
			printf("minishell: %s: No such file or directory", argv[1]);
			g_res[0] = 1;
		}
		else
			close (fd);
		return (-1);
	}
	if ((!ft_strncmp(argv[0], "<", 2) || !ft_strncmp(argv[0], "<<", 3))
		&& i == 2)
		return (-1);
	return (0);
}

int	check_redir(char **argv)
{
	int		i;
	t_redir	r;

	ft_bzero(&r, sizeof(r));
	if (if_first_redir(argv) < 0)
		return (-1);
	i = -1;
	while (argv[++i])
	{
		if (!ft_strncmp(argv[i], "<", 2) || !ft_strncmp(argv[i], "<<", 2))
		{
			r.rev_red++;
			if (r.rev_red > 1 || r.red > 0)
				return (print_syntax_err('<'));
		}
		else if (!ft_strncmp(argv[i], ">", 2) || !ft_strncmp(argv[i], ">>", 3))
		{
			r.red++;
			if (r.red > 1 || r.rev_red > 0)
				return (print_syntax_err('>'));
		}
		else
			ft_bzero(&r, sizeof(r));
	}
	return (0);
}

int	check_sym(char *argv, int *k, char *c)
{
	int	j;

	j = 0;
	while (argv[j])
	{
		if ((0 == *k || 3 == *k) && (argv[j] == '<' || argv[j] == '>'))
		{
			*c = argv[j];
			*k = 1;
		}
		if (argv[j] == ' ' && *k == 0)
			*k = 2;
		if (ft_isalnum(argv[j]) || argv[j] == '_' || '$' == argv[j])
			*k = 3;
		j++;
	}
	return (j);
}

int	check_argv(t_all *all, char **argv)
{
	int		i;
	int		k;
	char	c;
	int		j;

	i = 0;
	if (is_space_arg(argv) == 0 && all->cmds_num == 1)
		return (-1);
	while (argv[i])
	{
		k = 0;
		j = check_sym(argv[i], &k, &c);
		if (k == 1)
		{	
			printf("minishell: syntax error near unexpected token `%c'\n", c);
			return (-1);
		}
		if (k == 2 || j == 0)
		{
			printf("minishell: error multiline command\n");
			return (-1);
		}
		i++;
	}
	return (0);
}
