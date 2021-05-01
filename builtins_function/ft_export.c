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

void	export_plus()
{
	if (ft_strlen(cmd->argv[i]) > 1
		&& cmd->argv[i][first_space - 1] == '+')
	{
		key = ft_substr(cmd->argv[i], 0, first_space - 1);
		tmp = ft_substr(cmd->argv[i],
			++first_space, ft_strlen(cmd->argv[i]));
		value = search_key(exp, key);
		if (value == NULL)
			value = ft_strdup("");
		value = my_strjoin(value, tmp);
		add_key(envp, key, value);
		add_key(exp, key, value);
		continue;
	}
	else
		key = ft_substr(cmd->argv[i], 0, first_space);
}

void	ft_export_arg(t_cmd *cmd, t_list *envp, t_list *exp)
{
	int		len;
	int		i;
	char	*key;
	char	*value;
	char	*tmp;
	int		first_space;

	i = 0;
	len = len_arr(cmd->argv);
	while (++i < len && cmd->argv[i])
	{
		first_space = is_delim(cmd->argv[i]);
		if (first_space != 0)
		{
			// if (ft_strlen(cmd->argv[i]) > 1
			// 	&& cmd->argv[i][first_space - 1] == '+')
			// {
			// 	key = ft_substr(cmd->argv[i], 0, first_space - 1);
			// 	tmp = ft_substr(cmd->argv[i],
			// 			++first_space, ft_strlen(cmd->argv[i]));
			// 	value = search_key(exp, key);
			// 	if (value == NULL)
			// 		value = ft_strdup("");
			// 	value = my_strjoin(value, tmp);
			// 	add_key(envp, key, value);
			// 	add_key(exp, key, value);
			// 	continue;
			// }
			// else
			// 	key = ft_substr(cmd->argv[i], 0, first_space);
		}
		else
			key = ft_strdup(cmd->argv[i]);
		if (!is_valid_id(key))
		{
			printf("minishell: export `%s': not a valid identifier\n", key);
			gl_fd[0] = 1;
			free(key);
		}
		if (first_space != 0)
			value = ft_substr(cmd->argv[i],
					++first_space, ft_strlen(cmd->argv[i]));
		else
			value = NULL;
		add_key(envp, key, value);
		add_key(exp, key, value);
	}
}
