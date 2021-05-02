#include "includes/minishell.h"

char 	*get_env_val(t_list *envp, char *key)
{
	t_env *tmp;
	t_list *tmp_list;

	tmp_list = envp;
	while (tmp_list)
	{
		tmp = tmp_list->content;
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1 ))
			return (tmp->value);
		tmp_list = tmp_list->next;
	}
	return (NULL);
}



int		argv_len(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}


int		m_struct(t_all *all, char ***argv)
{
	int i;

	all->cmds = (t_cmd*)malloc(sizeof(t_cmd) * all->cmds_num);
	if (!all->cmds)
		return(print_merror(all));
	i = 0;
	while(i < all->cmds_num)
	{
		all->cmds[i].args_num = argv_len(argv[i]);
		all->cmds[i].argv = argv[i];
		i++;
	}
	return (0);
}

void	free_cmd(t_all *all)
{
	int i;
	int j;

	i = 0;
	if (all->cmds)
	{
		while (i < all->cmds_num)
		{
			j = 0;
			while(all->cmds[i].argv[j])
			{
				if (all->cmds[i].argv[j])
					free(all->cmds[i].argv[j]);
				j++;
			}
			free(all->cmds[i].argv);
			i++;
		}
		free(all->cmds);
		all->cmds = NULL;
	}
}