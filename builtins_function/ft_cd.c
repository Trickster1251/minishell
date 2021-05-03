#include "../includes/minishell.h"

void	search_and_destroy(t_list *lst, char *key)
{
	t_list		*tmp;
	t_env		*env;

	tmp = lst;
	while (tmp)
	{
		env = tmp->content;
		if (!ft_strncmp(key, env->key, ft_strlen(key)))
		{
			free(env->value);
			return ;
		}
		tmp = tmp->next;
	}
}

void	change_dir(t_cmd *cmd, t_list *lst, t_list *exp, char *tmp)
{
	char	*dir1;
	char	dir[1000];

	if (cmd->argv[1])
	{
		tmp = cmd->argv[1];
		if (!ft_strncmp(cmd->argv[1], "/", 1)
			&& ft_strncmp(cmd->argv[1], "/", 2))
			tmp++;
	}
	if ((chdir(tmp)) == -1)
	{
		printf("minishell: cd: %s\n", strerror(errno));
		g_res[0] = 1;
	}
	else
	{
		dir1 = ft_strdup(getcwd(dir, 1000));
		printf("this is pwd:=%s\n", dir1);
		search_and_destroy(lst, "PWD");
		add_key(lst, "PWD", dir1, 0);
	}
}

void	ft_cd(t_cmd *cmd, t_list *lst, t_list *exp)
{
	char	*pwd;

	pwd = NULL;
	if (!cmd->argv[1])
	{
		pwd = search_key(lst, "HOME");
		if (pwd == NULL)
		{
			printf("minishell: HOME not set\n");
			g_res[0] = 1;
			return ;
		}
	}
	change_dir(cmd, lst, exp, pwd);
	if (pwd)
		free(pwd);
	g_res[0] = 0;
}
