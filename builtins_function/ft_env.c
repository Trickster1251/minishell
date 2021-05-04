#include "../includes/minishell.h"

void	print_env(t_list *lst)
{
	t_list	*tmp;
	t_env	*env;

	tmp = lst;
	while (tmp)
	{
		env = tmp->content;
		if (env->value != NULL)
			printf("%s=%s\n", env->key, env->value);
		tmp = tmp->next;
	}
	g_res[0] = 0;
}
