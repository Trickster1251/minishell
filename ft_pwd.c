#include "includes/minishell.h"

void    ft_exit(t_cmd *cmd)
{
    printf("exit\n");
    if (cmd->argv[1] != NULL)
    {
        if (!ft_isdigit_str(cmd->argv[1]))
        {
            printf("minishell: exit: %s: numeric arguments required\n", cmd->argv[1]);
            gl_fd[0] = 255;
        }
        else if (cmd->argv[2] != NULL)
        {
            printf("minishell: exit: too many arguments\n");
            gl_fd[0] = 1;
        }
        else
            gl_fd[0] = ft_atoi(cmd->argv[1]);
    }
    exit(gl_fd[0]);
}

//Готово

void 	search_and_destroy(t_list *lst, char *key)
{
	t_list *tmp;
	t_env *env;

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

void    ft_cd(t_cmd *cmd, t_list *lst, t_list *exp)
{
    char *pwd;
	pwd = NULL;
	char *tmp;
    if (!cmd->argv[1])
    {
        pwd = search_key(lst , "HOME");
        if (pwd == NULL)
        {
            printf("minishell: HOME not set\n");
            gl_fd[0] = 1;
            return ;
        }
    }
    char dir[1000];
    char *dir1;

	if (cmd->argv[1])
	{
		tmp = cmd->argv[1];
		if (!ft_strncmp(cmd->argv[1], "/", 1) && ft_strncmp(cmd->argv[1], "/", 2))
			tmp++; 
		if ((chdir(tmp)) == -1)
		{
			printf("minishell: cd: %s\n", strerror(errno));
			gl_fd[0] = 1;
		}
		else
		{
			dir1 = ft_strdup(getcwd(dir, 1000));
			printf("this is pwd:=%s\n", dir1);
			//free(get_e)
			search_and_destroy(lst, "PWD");
			add_key(lst, "PWD", dir1);
			add_key(exp, "PWD", dir1);
		}
	}
	else
	{
		if ((chdir(pwd)) == -1)
		{
			printf("minishell: cd: %s\n", strerror(errno));
			gl_fd[0] = 1;
		}
		else
		{
			dir1 = ft_strdup(getcwd(dir, 1000));
			printf("this is pwd:=%s\n", dir1);
			search_and_destroy(lst, "PWD");
			add_key(lst, "PWD", dir1);
			add_key(exp, "PWD", dir1);
		}
	}
	if (pwd)
		free(pwd);
    gl_fd[0] = 0;
}

void    ft_env(t_cmd *cmd, t_list *envp)
{
    if (cmd->argv[1] == NULL)
        print_env(envp);
    else
    {
        printf("env: %s: No such file or directory\n", cmd->argv[1]);
        gl_fd[0] = 1;
    }
}

void    ft_pwd(t_cmd *cmd)
{
    char dir[1000];
    char *dir1 = getcwd(dir, 1000);

    if (cmd->argv[1] == NULL)
    {
        write(cmd->fd[1], dir1 ,strlen(dir1));
        write(cmd->fd[1], "\n", 1);
    }
    else
    {
        printf("pwd: too many arguments\n");
        gl_fd[0] = 1;
    }
}