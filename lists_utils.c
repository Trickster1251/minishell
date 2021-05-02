#include "includes/minishell.h"

int	set_value(t_list *lst, char *key, char *new_val)
{
	t_list	*tmp;
	t_env	*env;

	tmp = lst;
	while (tmp)
	{
		env = tmp->content;
		if (ft_strncmp(key, env->key, ft_strlen(key)) == 0)
		{
			env->value = new_val;
			return (0);
		}
		tmp = tmp->next;
	}
	return (1);
}

char	*search_key(t_list *lst, char *key)
{
	t_list	*tmp;
	t_env	*env;
	char	*result;

	tmp = lst;
	while (tmp)
	{
		env = tmp->content;
		if (ft_strncmp(key, env->key, ft_strlen(env->key)) == 0)
		{
			if (env->value != NULL)
				result = ft_strdup(env->value);
			else
				return (ft_strdup(""));
			return (result);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	add_key(t_list *lst, char *key, char *value, int f)
{
	t_env	*env;
	char	*tmp;

	env = ft_calloc(sizeof(t_env *), 2);
	env->key = key;
	env->value = value;
	tmp = search_key(lst, key);
	if (tmp == NULL)
		ft_lstadd_back(&lst, ft_lstnew(env));
	else
		set_value(lst, key, value);
	if (tmp != NULL)
	{
		if (f == 1)
			free(key);
		free(tmp);
		free(env);
	}
}

char	**lst_to_argv(t_list *lst)
{
	t_list	*tmp;
	char	**arr;
	int		i;

	i = 0;
	arr = ft_calloc(sizeof(char *), ft_lstsize(lst) + 1);
	tmp = lst;
	while (tmp)
	{
		arr[i++] = ft_strdup(tmp->content);
		tmp = tmp->next;
	}
	arr[i] = NULL;
	return (arr);
}

char	**lst_to_array(t_list *lst)
{
	t_list	*tmp;
	t_env	*env;
	int		i;
	char	*key;
	char	**arr;

	i = -1;
	tmp = lst;
	arr = ft_calloc(sizeof(char *), ft_lstsize(lst) + 1);
	while (tmp)
	{
		env = tmp->content;
		key = ft_strjoin(env->key, "=");
		arr[++i] = my_strjoin(key, env->value);
		tmp = tmp->next;
	}
	return (arr);
}
