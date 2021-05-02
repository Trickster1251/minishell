#include "includes/minishell.h"

void free_argv(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void free_struct(t_all *all, char **argv, char ***cmds)
{
	free(argv);
	free(cmds);
	free_cmd(all);
}

char	*if_no_var(t_all *all, char *str, char *d_pointer, char *end)
{
	char *tmp1;
	char *tmp2;

	tmp2 = get_env_val(all->envp, end);

	if (!tmp2)
	{
		tmp1 = ft_substr(str, 0, d_pointer - str);
		if (!tmp1)
		{
			print_merror(all);
			return (NULL);
		}
		if (ft_isalnum((int)*(d_pointer + 1)) || (char)*(d_pointer + 1) == '_')
			tmp1 = my_strjoin(tmp1, after_var(d_pointer + 1));
		else
		{
			*d_pointer *= -1;
			tmp1 = my_strjoin(tmp1, d_pointer);
		}
		if (!tmp1)
		{
			print_merror(all);
			return (NULL);
		}
		free(str);
		str = tmp1;
	}
	return (str);
}

int		make_struct(t_all *all, char *str)
{
	char **argv;
	int i;
	char **new_argv;
	char ***cmds;

	i = 0;
	argv = ft_split(str, '|');
	if (!argv)
		return (print_merror(all));
	all->cmds_num = argv_len(argv);
	if (check_argv(all, argv) < 0)
	{
		free_argv(argv);
		return (-1);
	}
	cmds = (char***)malloc(sizeof(char**) * (all->cmds_num + 1));
	if (!cmds)
		return(print_merror(all));
	cmds[all->cmds_num] = NULL;
	while (argv[i])
	{
		new_argv = tokenize(argv[i], all);
		if (!new_argv)
			return (-1);
		if (check_redir(new_argv) < 0)
		{
			free(argv);
			free_argv(new_argv);
			free(cmds);
			return (-1);
		}
		cmds[i] = new_argv;
		i++;
	}
	i = 0;
	if (m_struct(all, cmds) < 0)
		return (print_merror(all));
	execute_cmd(all);
	free_struct(all, argv, cmds);
	return (0);
}
