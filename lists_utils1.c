#include "includes/minishell.h"

void	free_str_arr(char **arr)
{
	int	i;

	i = -1;
	while (arr[++i])
		free(arr[i]);
	free(arr);
}

t_list *parse_env(char **env)
{
	t_list *lst;
	t_env *tmp;
	char *tmp_str;

	lst = NULL;
	while (*env)
	{
		tmp_str = *env;
		while (*tmp_str)
		{
			if (*tmp_str == '=')
			{
				tmp = (t_env *)malloc(sizeof(t_env));
				if (!tmp)
					return (NULL);
				tmp->key = ft_substr(*env, 0, tmp_str - *env);
				if (tmp->key != NULL)
					tmp->value = ft_strdup(tmp_str + 1);
				if (ft_strncmp(tmp->key, "OLDPWD", 7))
					ft_lstadd_back(&lst, ft_lstnew(tmp));
				else
					{
						free(tmp->key);
						free(tmp->value);
						free(tmp);
					}
				break;
			}
			tmp_str++;
		}
		env++;
	}
	return (lst);
}















// int	parse_item(char *tmp_str, char **env, t_list *lst)
// {
// 	t_env *tmp;
// 	while (*tmp_str)
// 	{
// 		if (*tmp_str == '=')
// 		{
// 			tmp = (t_env *)malloc(sizeof(t_env));
// 			if (!tmp)
// 				return (0);
// 			tmp->key = ft_substr(*env, 0, tmp_str - *env);
// 			if (tmp->key != NULL)
// 				tmp->value = ft_strdup(tmp_str + 1);
// 			if (ft_strncmp(tmp->key, "OLDPWD", 7))
// 				ft_lstadd_back(&lst, ft_lstnew(tmp));
// 			else
// 			{
// 				free(tmp->key);
// 				free(tmp->value);
// 				free(tmp);
// 			}
// 			break;
// 		}
// 		tmp_str++;
// 	}
// 	return (1);
// }

// t_list	*parse_env(char **env)
// {
// 	t_env *tmp;
// 	t_list *lst;
// 	char *tmp_str;

// 	lst = NULL;
// 	while (*env)
// 	{
// 		tmp_str = *env;
// 		if ((parse_item(tmp_str, env, lst) == 0))
// 			return (NULL);
// 		{
// 			if (*tmp_str == '=')
// 			{
// 				tmp = (t_env*)malloc(sizeof(t_env));
// 				if (!tmp)
// 					return (NULL);
// 				tmp->key = ft_substr(*env, 0, tmp_str - *env);
// 				if (tmp->key != NULL)
// 					tmp->value = ft_strdup(tmp_str + 1);
// 				if (ft_strncmp(tmp->key, "OLDPWD", 7))
// 					ft_lstadd_back(&lst, ft_lstnew(tmp));
// 				else
// 					{
// 						free(tmp->key);
// 						free(tmp->value);
// 						free(tmp);
// 					}
// 				break;
// 			}
// 			tmp_str++;
// 		}
// 		env++;
// 	}
// 	return (lst);
// }





// int	parse_item(t_env *tmp, char **env, t_list *lst)
// {
// 	char *tmp_str;

// 	tmp_str = *env;
// 	while (*tmp_str)
// 	{
// 		if (*tmp_str == '=')
// 		{
// 			tmp = (t_env *)malloc(sizeof(t_env));
// 			if (!tmp)
// 				return (0);
// 			tmp->key = ft_substr(*env, 0, tmp_str - *env);
// 			if (tmp->key != NULL)
// 				tmp->value = ft_strdup(tmp_str + 1);
// 			if (ft_strncmp(tmp->key, "OLDPWD", 7))
// 				ft_lstadd_back(&lst, ft_lstnew(tmp));
// 			else
// 			{
// 				free(tmp->key);
// 				free(tmp->value);
// 				free(tmp);
// 			}
// 		}
// 		tmp_str++;
// 	}
// 	return (1);
// }

// t_list *parse_env(char **env)
// {
// 	t_list *lst;
// 	t_env *tmp;


// 	lst = NULL;
// 	while (*env)
// 	{
// 		if(!parse_item(tmp, env, lst))
// 			return (NULL);
// 		env++;
// 	}
// 	return (lst);
// }

void	delete_item(t_list *tmp, char *key, t_env *env)
{
	while (tmp)
	{
		if (tmp->next != NULL)
		{
			env = tmp->next->content;
			if (!ft_strncmp(env->key, key, ft_strlen(key)))
			{
				free((t_env *)(env)->key);
				free((t_env *)(env)->value);
				free(tmp->next->content);
				free((tmp->next));
				tmp->next = tmp->next->next;
				return ;
			}
		}
		tmp = tmp->next;
	}
}

void	del_key(t_list **lst, char *key)
{
	t_list	*tmp;
	t_list	*tmp_next;
	t_env	*env;

	tmp = *lst;
	env = tmp->content;
	tmp_next = tmp->next;
	if (!ft_strncmp(env->key, key, ft_strlen(key)))
	{
		free((t_env *)(env)->key);
		free((t_env *)(env)->value);
		free(tmp->content);
		tmp = tmp_next;
		free(tmp);
		return ;
	}
	else
		delete_item(tmp, key, env);
}

int	is_valid_id(char *key)
{
	int	i;

	i = 0;
	while (key[i])
	{
		if (!ft_isalpha(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
