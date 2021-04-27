#include "includes/minishell.h"

// Лики в unset
// Сделать сортировку в export, и возможность канкатенировать переменные

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

    // && (ft_strncmp(cmd->argv[0], "cat\0", 4))
    if (pfd == 0)
        return (0);
    if (i == 0)
    {
        printf("Gatcha!\n");
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

char     *search_path(t_all *all, t_cmd *cmd, t_list *envp)
{
    int     i;

    i = -1;
    char    **env = lst_to_array(envp);
    char    **path = ft_split(search_key(envp, "PATH"), ':');
    char    *comd = ft_strjoin("/", cmd->argv[0]);

    struct stat buf;

    gl_fd[0] = 0;
    if (lstat(cmd->argv[0], &buf) == 0)
        return (cmd->argv[0]);
    while(path[++i] && cmd->fd[0] != -1 && cmd->fd[1] != -1)
    {
        if (lstat(ft_strjoin(path[i], comd), &buf) == 0)
        {
            char *tmp = ft_strjoin(path[i], comd);
            return (tmp);
        }
    }
    printf("minishell: %s: command not found\n", cmd->argv[0]);
    gl_fd[0] = 127;
    return (NULL);
}

void    exec_command(t_all *all, t_cmd *cmd, t_list *envp, char *path)
{
    char    **env = lst_to_array(envp);
    struct stat buf;
    char    *comd = ft_strjoin("/", cmd->argv[0]);

	
    if (lstat(cmd->argv[0], &buf) == 0)
        execve(cmd->argv[0], cmd->argv, env);
    if (lstat(path, &buf) == 0)
        execve(path, cmd->argv, env);
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

int is_redir_type(char *str)
{
    return (ft_strncmp(str, ">\0", 2) == 0 || ft_strncmp(str, ">>\0", 3) == 0 ||
    ft_strncmp(str, "<", 2) == 0);
}

void    create_open_fd(t_all *a, t_cmd *cmd, char **arr)
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
        gl_fd[0] = 123;
    }
}

int    **pipes_fd(t_all *a)
{
    int **pfd;
    pfd = calloc(sizeof(int *), (a->cmds_num - 1));
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
    pid_t pid[a->cmds_num];
	int status[a->cmds_num];
	int f;

    while(++i < a->cmds_num)
    {
        a->cmds[i].fd[0] = 0;
        a->cmds[i].fd[1] = 1;
        a->cmds[i].count_redir = 0;
        a->cmds[i].count_redir = count_redir(a->cmds[i].argv);
        if (a->cmds[i].count_redir != 0)
            create_open_fd(a, &a->cmds[i], a->cmds[i].argv);
        // exec builtins commands
        // если переходить по / то сега, если просто cd, то ошибка малока
        if (strncmp(a->cmds[i].argv[0], "cd\0", 3) == 0)
        {
            printf("Gatcha!!!\n");
            ft_cd (&a->cmds[i], a->envp);
        }
        else if (strncmp(a->cmds[i].argv[0], "echo\0", 5) == 0)
        {
            pid[i] = fork();
            if (pid[i] != 0)
            {
                if (pfd != NULL && i < a->cmds_num  -1)
                    close(pfd[i][1]);
            }
            if (pid[i] == 0)
            {
                if (a->cmds_num > 1)
                    dup_func(pfd, i, a->cmds_num, a->cmds);
                else
                {
                    dup2(a->cmds[i].fd[0], 0);
                    dup2(a->cmds[i].fd[1], 1);
                }
                 ft_echo(a->cmds);
                 exit (0);
            }
        }
        else if (strncmp(a->cmds[i].argv[0], "pwd\0", 4) == 0)
            ft_pwd(a->cmds);
        else if (strncmp(a->cmds[i].argv[0], "env\0", 4) == 0)
        {

            pid[i] = fork();
            if (pid[i] != 0)
            {
                if (pfd != NULL && i < a->cmds_num  -1)
                    close(pfd[i][1]);
            }
            if (pid[i] == 0)
            {
                if (a->cmds_num > 1)
                    dup_func(pfd, i, a->cmds_num, a->cmds);
                else
                {
                    dup2(a->cmds[i].fd[0], 0);
                    dup2(a->cmds[i].fd[1], 1);
                }
                ft_env(a->cmds, a->envp);
                exit (0);
            }

        }
        else if (strncmp(a->cmds[i].argv[0], "export\0", 7) == 0)
        {
			if (a->cmds[i].argv[1] == NULL)
			{
				pid[i] = fork();
				if (pid[i] != 0)
				{
					if (pfd != NULL && i < a->cmds_num - 1)
						close(pfd[i][1]);
				}
				if (pid[i] == 0)
				{
					if (a->cmds_num > 1)
						dup_func(pfd, i, a->cmds_num, a->cmds);
					else
					{
						dup2(a->cmds[i].fd[0], 0);
						dup2(a->cmds[i].fd[1], 1);
					}
					ft_export(a->cmds, a->envp, a->exp);
					exit(0);
				}
			}
			else
				ft_export_arg(a->cmds, a->envp, a->exp);
		}
        else if (strncmp(a->cmds[i].argv[0], "unset\0", 6) == 0)
        {
            ft_unset(a->cmds, a->envp, a->exp);
        }
        else if (strncmp(a->cmds[i].argv[0], "exit\0", 6) == 0)
            ft_exit(a->cmds);
        else
        {
            char *path = search_path(a, &a->cmds[i], a->envp);
            pid[i] = fork();
            if (pid[i] != 0)
            {
                if (pfd != NULL && i < a->cmds_num  -1)
                {
                    printf("CLOSE FD[1]\n");
                    close(pfd[i][1]);
                }
            }
            if (pid[i] == 0)
            {
                if (a->cmds_num > 1)
                    dup_func(pfd, i, a->cmds_num, a->cmds);
                else
                {
                    dup2(a->cmds[i].fd[0], 0);
                    dup2(a->cmds[i].fd[1], 1);
                }
                if (path == NULL)
                    exit(127);
                exec_command(a, &a->cmds[i], a->envp, path);

            // signal(2, ctrl_c);
            // signal(3, ctrl_slash);
            }
			// waitpid(pid[i], &status[i], 0);
			// f = WSTOPSIG(status[i]);
			// if (f != 0)
			// 	gl_fd[0] = f;
        }
        for (int j = 0; j < a->cmds_num; j++)
		{
			waitpid(pid[j], &status[j], 0);
			f = WSTOPSIG(status[j]);
			if (f != 0)
				gl_fd[0] = f;
		}

        // printf("code : %d\n", gl_fd[0]);
    // write(1, "EXEC FINISH\n", 12);
//                sleep(1);
}