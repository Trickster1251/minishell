#include "includes/minishell.h"

// Лики в unset
// Сделать сортировку в export, и возможность канкатенировать переменные
// Сделать обратный редирект, если встречен обратный редирект, то dup2, заменяет 0 на fd[0]
// Начать делать пайпы
// Если есть пайпы, то я считаюх их количество, и спличу по-ним
// Инициализация массивов пайпов, кол-во элементов определяется количеством элементов


// "cat < a | grep "hello" | wc"
// Мне попадает строка, она сплиться по пайпам, потом по пробелам, получается это

int     count_pipes(char *s, t_cmd *cmd)
{
    int     i = -1;
    int     count = 0;
    while(s[++i])
    {
        if (s[i] == '|')
            count++;
    }
    return (count);
}

void    print_arr(char **arr)
{
    int     i;
    int     j;

    i = -1;
    while(arr[++i])
    {
        j = -1;
        while(arr[i][++j])
        {
            write(1,&arr[i][j],1);
        }
        write(1,"\n",1);
    }
}

int     dup_func(int **pfd, int i, int cmd_com, t_cmd *cmd)
{
    if (pfd == 0)
        return (0);
    if (i == 0)
    {
        close(pfd[i][0]);
        dup2(pfd[i][1], 1);
        close(pfd[i][1]);
    }
    else if (i < cmd_com - 1)
    {
        close(pfd[i - 1][1]);
        dup2(pfd[i - 1][0], 0);
        close(pfd[i][0]);
        dup2(pfd[i][1], 1);
    }
    else
    {
        // printf("AAAAAAAAAAAAA\n");
        close(pfd[i - 1][1]);
        dup2(pfd[i - 1][0], 0);
        close(pfd[i - 1][1]);
    }
    if (cmd[i].fd[0] != 0)
        dup2(cmd[i].fd[0], 0);
    if (cmd[i].fd[1] != 1)
        dup2(cmd[i].fd[1], 1);
    return (0);
}

void    exec_command(t_all *all, t_cmd *cmd, int j, t_list *envp)
{
    char    **env = lst_to_array(envp);
    char    **path = ft_split(search_key(envp, "PATH"), ':');
    char    *comd = ft_strjoin("/", cmd->argv[0]);
    int     i;

    i = -1;
    struct stat buf;

    printf("Вошел!\n");
    if (lstat(cmd->argv[0], &buf) == 0)
    {
        execve(cmd->argv[0], cmd->argv, env);
    }
    // while(path[++i])
    while(path[++i] && cmd->fd[0] != -1 && cmd->fd[1] != -1)
    {
        write(1, "EXEC FINISH\n", 12);
        if (lstat(ft_strjoin(path[i], comd), &buf) == 0)
        {
            char *tmp = ft_strjoin(path[i], comd);
            printf("PATH: |%s|\nCMD ARG: |%s| |%s|\n", tmp, cmd->argv[0], cmd->argv[1]);
            write(1, "EXEC FINISH!!!\n", 12);
            execve(tmp, cmd->argv, env);
        }
    }
    printf("minishell: %s: command not found\n", cmd->argv[0]);
    gl_fd[0] = 127;
}

int     count_redir(char **arr)
{
    int     i;
    int     count;

    i = -1;
    count = 0;
    while(arr[++i])
    {
        if (ft_strncmp(arr[i], ">\0", 2) == 0 ||
        ft_strncmp(arr[i], ">>\0", 3) == 0 ||
        ft_strncmp(arr[i], "<\0", 2) == 0)
            count++;
    }
    return (count);
}

int    ft_redir(t_cmd *cmd, char **arr,  int i, int count_redir)
{
    if (ft_strncmp(arr[i - 1], ">\0", 2) == 0)
        {
            count_redir++;
            if (count_redir < cmd->count_redir)
                open(arr[i], O_CREAT | O_TRUNC);
            else
                cmd->fd[1] = open(arr[i], O_TRUNC | O_RDWR | O_CREAT);
        }
    else if (ft_strncmp(arr[i - 1], ">>\0", 3) == 0)
        {
            count_redir++;
            if (count_redir < cmd->count_redir)
                open(arr[i], O_CREAT);
            else
                cmd->fd[1] = open(arr[i], O_APPEND | O_RDWR | O_CREAT);
        }
    return (count_redir);
}

int is_redir_type(char *str)
{
    return (ft_strncmp(str, ">\0", 2) == 0 || ft_strncmp(str, ">>\0", 3) == 0 ||
    ft_strncmp(str, "<", 2) == 0);
}

void    create_open_fd(t_cmd *cmd, char **arr)
{
    int     i;
    t_list  *lst = NULL;
    i = 0;
    if (!is_redir_type(arr[i]))
    {
        printf("Записал в стркуктуру %s------\n", arr[i]);
        ft_lstadd_back(&lst, ft_lstnew(arr[i]));
    }
    i++;

    while(arr[i])
    {
        if (!is_redir_type(arr[i]) && !is_redir_type(arr[i - 1]))
        {
            printf("Записал в стркуктуру %s---\n", arr[i]);
            ft_lstadd_back(&lst, ft_lstnew(arr[i]));
        }
        else
        {
            if (ft_strncmp(arr[i - 1], "<\0", 2) == 0)
            {
                if (cmd->fd[0] != 0)
                    close(cmd->fd[0]);
                printf("Обратный редирект %s --> %s\n", arr[i - 1], arr[i]);
                cmd->fd[0] = open(arr[i], O_RDONLY);
            }
            else if (ft_strncmp(arr[i - 1], ">\0", 2) == 0)
            {
                if (cmd->fd[1] != 1)
                    close(cmd->fd[1]);
                printf("Редирект %s --> %s\n", arr[i - 1], arr[i]);
                cmd->fd[1] = open(arr[i], O_TRUNC | O_RDWR | O_CREAT, 0666);
            }
            else if (ft_strncmp(arr[i - 1], ">>\0", 3) == 0)
            {
                if (cmd->fd[1] != 1)
                    close(cmd->fd[1]);
                printf("Редирект %s --> %s\n", arr[i - 1], arr[i]);
                cmd->fd[1] = open(arr[i], O_APPEND | O_RDWR | O_CREAT, 0666);
            }
            if (errno != 0)
            {
                printf("%s\n", strerror(errno));
                errno = 0;
                break ;
            }
        }
        i++;
        // printf("iteration %i\n", i - 1);
    }
    cmd->argv = lst_to_argv(lst);
}

void    ctrl_slash(int sig)
{
    if (sig == 3)
    {
        printf("^\\Quit: 3\n");
        exit (127);
    }
}

void    ctrl_c(int sig)
{
    if (sig == 2)
    {
        printf("^C\n");
        exit (123);
    }
}

int    **pipes_fd(t_all *a)
{
    int **pfd;
    pfd = malloc(sizeof(int *) * (a->cmds_num - 1));
    for (int j = 0; j < a->cmds_num - 1; j++)
    {
        pfd[j] = malloc(sizeof(int) * 2);
        pipe(pfd[j]);
    }
    return (pfd);
}

void     execute_cmd(t_all *a)
{

    a->exp = a->envp;
    //Тут 2 лика, не фришу старое значение
//    init_shlvl(a->envp, a->exp);
    int     i;
    int     **pfd;
    pfd = 0;
    if (a->cmds_num > 1)
        pfd = pipes_fd(a);
    i = -1;
    pid_t pid;

    while(++i < a->cmds_num)
    {
        a->cmds[i].fd[0] = 0;
        a->cmds[i].fd[1] = 1;
        a->cmds[i].count_redir = 0;
        // print_arr(a->cmds[i].argv);
        a->cmds[i].count_redir = count_redir(a->cmds[i].argv);
        if (a->cmds[i].count_redir != 0)
            create_open_fd(&a->cmds[i], a->cmds[i].argv);

        // exec builtins commands

        // если переходить по / то сега, если просто cd, то ошибка малока
        if (strncmp(a->cmds[i].argv[0], "cd\0", 3) == 0)
        {
            printf("Gatcha!!!\n");
            ft_cd(&a->cmds[i], a->envp);
            // ft_env(a->cmds, a->envp);
        }
        else if (strncmp(a->cmds[i].argv[0], "echo\0", 5) == 0)
        {
            // write(1, "asd\n", 4);
            ft_echo(a->cmds);
        }
        else if (strncmp(a->cmds[i].argv[0], "pwd\0", 4) == 0)
            ft_pwd(a->cmds);
        else if (strncmp(a->cmds[i].argv[0], "env\0", 4) == 0)
            ft_env(a->cmds, a->envp);
        else if (strncmp(a->cmds[i].argv[0], "export\0", 7) == 0)
        {
            ft_export(a->cmds, a->exp, a->envp);
//            ft_env(a->cmds, a->envp);
            a->cmds[i].argv[1] = NULL;
            // ft_export(a->cmds, a->exp, a->envp);
//            printf("------------>%s\n", search_key(a->envp, a->cmds[i].argv[1]));
        }
        else if (strncmp(a->cmds[i].argv[0], "unset\0", 6) == 0)
        {
            ft_unset(a->cmds, a->envp, a->exp);
            // a->cmds[i].argv[1] = NULL;
            // ft_export(a->cmds, a->exp, a->envp);
            // ft_env(a->cmds, a->envp);
        }
        else if (strncmp(a->cmds[i].argv[0], "exit\0", 6) == 0)
            ft_exit(a->cmds);
        else if (a->cmds[i].argv[0] != NULL)
        {
            if ((pid = fork()) != 0)
            {
                close(pfd[i][1]);
            }
            else
            {
                printf("CMD NUM: %d\n", a->cmds_num);
                dup_func(pfd, i, a->cmds_num, a->cmds);
                exec_command(a, &a->cmds[i], i, a->envp);
                write(1, "EXEC FINISH1\n", 12);
                // signal(2, ctrl_c);
                // signal(3, ctrl_slash);
            }
        }
    }
    for (int j = 0; j < a->cmds_num; j++)
        waitpid(pid, 0, -1);
    // write(1, "EXEC FINISH\n", 12);
        //    while(1);
//                sleep(1);
}