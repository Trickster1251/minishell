#include "../includes/minishell.h"

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
