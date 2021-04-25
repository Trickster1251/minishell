#include "includes/minishell.h"

//Готов
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
void    ft_cd(t_cmd *cmd, t_list *lst)
{

    printf("arr = %s\n", cmd->argv[1]);
    if (cmd->argv[1] == NULL)
    {
        char *str= search_key(lst , "HOME\0");
        printf("arr = %s\n", cmd->argv[1]);
        if (!str)
        {
            printf("minishell: HOME not set\n");
            gl_fd[0] = 1;
        }
    }
    if ((chdir(cmd->argv[1])) == -1)
    {
        printf("minishell: cd: %s\n", strerror(errno));
        gl_fd[0] = 1;
    }
}

//Готово
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

//Готово
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