#include "includes/minishell.h"

int     len_arr(char **str)
{
    int i;

    i = -1;
    while(str[++i]);
    return (i);
}

int     is_min(int n, int m)
{
    if (n >= m)
        return (m);
    return (n);
}

int     is_max(int n, int m)
{
    if (n >= m)
        return (n);
    return (m);
}

int     is_delim(char *line)
{
    int     i;

    i = -1;
    while(line[++i])
    {
        if (line[i] == '=')
            return (i);
    }
    return (0);
}

char    **lst_to_array(t_list *lst)
{
    t_list *tmp = lst;
    t_env *env;
    int i = 0;
    int len_str;
    char *key;
    int len = ft_lstsize(lst);
    char **arr = ft_calloc(sizeof(char*), ft_lstsize(lst) + 1);

    while(tmp)
    {
        env = tmp->content;
        // len_str = ft_strlen(env->key) + ft_strlen(env->value) + 2;
        arr[i] = ft_strdup("");
        // arr[i] = ft_calloc(sizeof(char), len_str);
        // bzero(arr[i], len_str);
        key = ft_strjoin(env->key, "=");
        arr[i] = ft_strjoin(key, env->value);
        tmp = tmp->next;
        i++;
    }
    return(arr);
}

char    **lst_to_argv(t_list *lst)
{
    t_list *tmp = lst;
    int i = 0;
    int len_str;
    char *key;
    char **arr = ft_calloc(sizeof(char*), ft_lstsize(lst) + 1);

    while(tmp)
    {
        len_str = ft_strlen(tmp->content) + 1;
        arr[i] = ft_calloc(sizeof(char), len_str);
        bzero(arr[i], len_str);
        arr[i] = tmp->content;
        tmp = tmp->next;
        i++;
    }
    arr[i] = NULL;
    return(arr);
}
