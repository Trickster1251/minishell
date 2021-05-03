#include "../includes/minishell.h"

void	print_export(t_list **lst)
{
	t_list	*tmp;
	t_env	*env;

	tmp = (*lst);
	while (tmp)
	{
		env = tmp->content;
		if (env->value == NULL)
			printf("declare -x %s\n", env->key);
		else
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		tmp = tmp->next;
	}
}

typedef struct s_exp
{
	int		f_space;
	char	*key;
	char	*value;
	char	*tmp;
}			t_exp;

void	export_plus(t_exp *e, t_cmd *cmd, int i, t_all *a)
{
	if (ft_strlen(cmd->argv[i]) > 1
		&& cmd->argv[i][e->f_space - 1] == '+')
	{
		e->key = ft_substr(cmd->argv[i], 0, e->f_space - 1);
		e->tmp = ft_substr(cmd->argv[i],
				++e->f_space, ft_strlen(cmd->argv[i]));
		e->value = search_key(a->exp, e->key);
		if (e->value == NULL)
			e->value = ft_strdup("");
		e->value = my_strjoin(e->value, e->tmp);
		search_and_destroy(a->envp, e->key);
		add_key(a->envp, e->key, e->value, 1);
		free(e->tmp);
		e->tmp = NULL;
		return ;
	}
	else
		e->key = ft_substr(cmd->argv[i], 0, e->f_space);
}

void	ft_export_arg(t_cmd *cmd, t_all *a)
{
	int		i;
	t_exp	e;

	i = 0;
	while (++i < len_arr(cmd->argv) && cmd->argv[i])
	{
		e.f_space = is_delim(cmd->argv[i]);
		if (e.f_space != 0)
			export_plus(&e, cmd, i, a);
		else
			e.key = ft_strdup(cmd->argv[i]);
		if (!is_valid_id(e.key))
		{
			printf("minishell: export `%s': not a valid identifier\n", e.key);
			g_res[0] = 1;
			free(e.key);
		}
		if (e.f_space != 0)
			e.value = ft_substr(cmd->argv[i],
					++e.f_space, ft_strlen(cmd->argv[i]));
		else
			e.value = NULL;
		search_and_destroy(a->envp, e.key);
		add_key(a->envp, e.key, e.value, 1);
	}
}
