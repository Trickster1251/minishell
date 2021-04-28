#include "includes/minishell.h"

int     set_value(t_list *lst, char *key, char *new_val)
{
    t_list *tmp = lst;
    t_env *env;

    while(tmp)
    {
        env = tmp->content;
        if (ft_strncmp(key, env->key, ft_strlen(key)) == 0)
        {
            env->value = new_val;
            return(0);
        }
        tmp = tmp->next;
    }
    return(1);
}

char    *search_key(t_list *lst, char *key)
{
    t_list  *tmp;
    t_env   *env;
    char    *result;

    tmp = lst;
    while(tmp)
    {
        env = tmp->content;
        if (ft_strncmp(key, env->key, ft_strlen(env->key)) == 0)
        {
			if (env->value != NULL)
            	result = ft_strdup(env->value);
			else
				return (ft_strdup(""));
            // write(1,result,ft_strlen(result));
            // write(1,"\n", 1);
            // free(env->value);
            // free(env->key);
            return(result);
        }
        tmp = tmp->next;
    }
    return(NULL);
}

void    add_key(t_list *lst, char *key, char *value)
{
    t_env *env;
	char *tmp;

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
		free(tmp);
		free(env);
	}
}

void    print_lst(t_list *lst)
{
    t_list *tmp = lst;

    while(tmp)
    {
        printf("%s\n", tmp->content);
        tmp = tmp->next;
    }
}

//Изменить отображение ключа без значения
void    print_env(t_list *lst)
{
    t_list *tmp = lst;
    t_env *env;

    while(tmp)
    {
        env = tmp->content;
		if (env->value != NULL)
        	printf("%s=%s\n", env->key, env->value);
        tmp = tmp->next;
    }
}


int     who_more(char *str, char *str1)
{
    if (ft_strlen(str) >= ft_strlen(str1))
        return (ft_strlen(str));
    return (ft_strlen(str1));
}

void    print_export(t_list **lst)
{
    t_list *tmp = (*lst);
    t_env *env;

    while(tmp)
    {
        env = tmp->content;
		if (env->value == NULL)
			printf("declare -x %s\n", env->key);
		else
        	printf("declare -x %s=\"%s\"\n", env->key, env->value);
        tmp = tmp->next;
    }
}
