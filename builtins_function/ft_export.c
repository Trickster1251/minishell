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

// void	export_plus(int first_space, t_all *a, char *key, char *value, int i)
// {
// 	if (first_space != 0)
// 	{
// 		if (ft_strlen(cmd->argv[i]) > 1 && cmd->argv[i][first_space - 1] == '+')
// 		{
// 			key = ft_substr(cmd->argv[i], 0, first_space - 1);
// 			tmp = ft_substr(cmd->argv[i],
// 							++first_space, ft_strlen(cmd->argv[i]));
// 			value = search_key(a->exp, key);
// 			if (value == NULL)
// 				value = ft_strdup("");
// 			value = my_strjoin(value, tmp);
// 			search_and_destroy(a->envp, key);
// 			add_key(a->envp, key, value, 1);
// 			free(tmp);
// 			tmp = NULL;
// 			continue;
// 		}
// 		else
// 			key = ft_substr(cmd->argv[i], 0, first_space);
// 	}
// 	else
// 		key = ft_strdup(cmd->argv[i]);
// }

// void	ft_export_arg(t_cmd *cmd, t_all *a)
// {
// 	// int		len;
// 	int		i;
// 		*key;
// 	char	*value;
// 	char	*tmp;
// 	int		first_space;

// 	i = 0;
// 	// len = len_arr(cmd->argv);
// 	while (++i < len_arr(cmd->argv) && cmd->argv[i])
// 	{
// 		first_space = is_delim(cmd->argv[i]);
// 		if (first_space != 0)
// 		{
// 			if (ft_strlen(cmd->argv[i]) > 1
// 				&& cmd->argv[i][first_space - 1] == '+')
// 			{
// 				key = ft_substr(cmd->argv[i], 0, first_space - 1);
// 				tmp = ft_substr(cmd->argv[i],
// 						++first_space, ft_strlen(cmd->argv[i]));
// 				value = search_key(a->exp, key);
// 				if (value == NULL)
// 					value = ft_strdup("");
// 				value = my_strjoin(value, tmp);
// 				search_and_destroy(a->envp, key);
// 				add_key(a->envp, key, value, 1);
// 				free(tmp);
// 				tmp = NULL;
// 				continue;
// 			}
// 			else
// 				key = ft_substr(cmd->argv[i], 0, first_space);
// 		}
// 		else
// 			key = ft_strdup(cmd->argv[i]);
// 		if (!is_valid_id(key))
// 		{
// 			printf("minishell: export `%s': not a valid identifier\n", key);
// 			gl_fd[0] = 1;
// 			free(key);
// 		}
// 		if (first_space != 0)
// 			value = ft_substr(cmd->argv[i],
// 					++first_space, ft_strlen(cmd->argv[i]));
// 		else
// 			value = NULL;
// 		search_and_destroy(a->envp, key);
// 		add_key(a->envp, key, value, 1);
// 	}
// }

void	ft_export_arg(t_cmd *cmd, t_all *a)
{
	// int		len;
	int		i;
	char	*key;
	char	*value;
	char	*tmp;
	int		first_space;

	i = 0;
	// len = len_arr(cmd->argv);
	while (++i < len_arr(cmd->argv) && cmd->argv[i])
	{
		first_space = is_delim(cmd->argv[i]);
		if (first_space != 0)
		{
			if (ft_strlen(cmd->argv[i]) > 1
				&& cmd->argv[i][first_space - 1] == '+')
			{
				key = ft_substr(cmd->argv[i], 0, first_space - 1);
				tmp = ft_substr(cmd->argv[i],
						++first_space, ft_strlen(cmd->argv[i]));
				value = search_key(a->exp, key);
				if (value == NULL)
					value = ft_strdup("");
				value = my_strjoin(value, tmp);
				search_and_destroy(a->envp, key);
				add_key(a->envp, key, value, 1);
				free(tmp);
				tmp = NULL;
				continue;
			}
			else
				key = ft_substr(cmd->argv[i], 0, first_space);
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
		search_and_destroy(a->envp, key);
		add_key(a->envp, key, value, 1);
	}
}