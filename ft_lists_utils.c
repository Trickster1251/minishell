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
            result = ft_strdup(env->value);
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
	free(tmp);
//    free(env->value);
//    free(env->key);
    //free(env);
}

t_list  *sorting(t_list *lst)
{
    char    **arr;
    t_list  *tmp = lst;
    int     i;
    char    *min;

    arr = lst_to_array(lst);
    i = -1;
    while(arr[++i]);
    arr = ft_sort(arr, i);
    lst = parse_env(arr);
    return (lst);
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
		if (ft_strlen(env->value) > 0)
        	printf("%s=%s\n", env->key, env->value);
        tmp = tmp->next;
    }
}

//Изменить отображение ключа без значения
// export hello; в export появиться без равно, в env не отобразиться
// Если export hello=, то в export появиться 'hello=""', в env "hello="

void    print_export(t_list **lst)
{
    t_list *tmp = (*lst);
    t_env *env;


    while(tmp)
    {
        env = tmp->content;
		if (ft_strlen(env->value) > 0)
        	printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
        tmp = tmp->next;
    }
}
