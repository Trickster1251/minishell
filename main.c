#include "includes/minishell.h"
#include <string.h>


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
				tmp = (t_env*)malloc(sizeof(t_env));
				if (!tmp)
					return (NULL);
				tmp->key = ft_substr(*env, 0, tmp_str - *env);
				tmp->value = ft_strdup(tmp_str + 1);
				ft_lstadd_back(&lst, ft_lstnew(tmp));
				break;
			}
			tmp_str++;
		}
		env++;
	}
	return (lst);
}

// int words_len(char **words)
// {
// 	int i;

// 	i = 0;
// 	if (**words == NULL)
// 		return (0);
// 	while (words[i])
// 		i++;
// 	return (i);
// }

// char **wordsdup(char** words)
// {
// 	char **copy;

// 	int i;

// 	i = words_len(words);
// 	copy = (char**)ft_calloc(sizeof(char *), i);

// 	while(*words)
// 	{
		
// 	}
// }

t_all *init_all(char **env)
{
	t_all *all;

	all = (t_all*)malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	//all->env_strs = ft_strdup(env);
	all->env = parse_env(env);
	return (all);
}

int		main(int ac, char **av, char **env)
{
	t_all *all;
	t_env *tmp;

	all = init_all(env);
	while (all->env)
	{
		tmp = all->env->content;
		printf("%s=%s\n", tmp->key, tmp->value);
		all->env = all->env->next;
	}
}