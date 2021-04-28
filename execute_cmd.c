#include "includes/minishell.h"

void    print_arr(char **arr)
{
    int     i;
    int     j;

    i = -1;
    while(arr[++i])
    {
        j = -1;
        while(arr[i][++j])
            write(1,&arr[i][j],1);
        write(1,"\n",1);
    }
}

char     *search_path(t_all *all, t_cmd *cmd, t_list *envp)
{
    int     i;
    struct stat buf;
    char    **env;
    char    **path;
    char    *comd;

    path = ft_split(search_key(envp, "PATH"), ':');
    comd = ft_strjoin("/", cmd->argv[0]);
    env = lst_to_array(envp);
    i = -1;
    gl_fd[0] = 0;

    int     fd;

    while(path[++i] && cmd->fd[0] != -1 && cmd->fd[1] != -1)
    {
        if (lstat(ft_strjoin(path[i], comd), &buf) == 0)
        {
            char *tmp = ft_strjoin(path[i], comd);
            return (tmp);
        }
    }
    if (lstat(cmd->argv[0], &buf) == 0)
    {
        fd = open(cmd->argv[0], O_RDONLY);
        if (fd)
        {
            if (ft_strncmp("./", cmd->argv[0], 2))
            {
                printf("minishell: %s: command not found\n", cmd->argv[0]);
                gl_fd[0] = 127;
                return (NULL);
            }
        }
        return (cmd->argv[0]);
    }
    printf("minishell: %s: command not found\n", cmd->argv[0]);
    gl_fd[0] = 127;
    return (NULL);
}

void    exec_command(t_all *all, t_cmd *cmd, t_list *envp, char *path)
{
    char        **env = lst_to_array(envp);
    char        *comd = ft_strjoin("/", cmd->argv[0]);
    struct stat buf;
	
    if (lstat(path, &buf) == 0)
        execve(path, cmd->argv, env);
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

void    forking(pid_t *pid, int i, int ***fd, t_all *a)
{
    int     **pfd = *fd;
    if (pid[i] != 0)
    {
        if (pfd != NULL && i < a->cmds_num - 1)
        {
            dup2(a->cmds[i].fd[1], 1);
            close(pfd[i][1]);
        }
    }
    else if (pid[i] == 0)
    {
        if (a->cmds_num > 1)
            dup_fd(pfd, i, a->cmds_num, a->cmds);
        else
        {
            dup2(a->cmds[i].fd[0], 0);
            dup2(a->cmds[i].fd[1], 1);
        }
    }
    return ;
}

////////////
////////////
////////////
////////////

void     execute_cmd(t_all *a)
{
    int     i;
    int     **pfd;
	int     f;
    pid_t   pid[a->cmds_num];
	int     status[a->cmds_num];

	int j;
    i = -1;
    pfd = 0;
    a->exp = a->envp;
    if (a->cmds_num > 1)
        pfd = pipes_fd(a);
    while(++i < a->cmds_num)
    {
        a->cmds[i].fd[0] = 0;
        a->cmds[i].fd[1] = 1;
        a->cmds[i].count_redir = 0;
        a->cmds[i].count_redir = count_redir(a->cmds[i].argv);
        if (a->cmds[i].count_redir != 0)
            create_open_fd(a, &a->cmds[i], a->cmds[i].argv);
		j = 0;
		while (a->cmds[i].argv[j])
		{
			unshield(a->cmds[i].argv[j]);
			j++;
		}
        ///
        ///
        if (strncmp(a->cmds[i].argv[0], "cd\0", 3) == 0)
            ft_cd (&a->cmds[i], a->envp, a->exp);
        else if (strncmp(a->cmds[i].argv[0], "echo\0", 5) == 0)
        {
            pid[i] = fork();
            forking(pid, i, &pfd, a);
            if (pid[i] == 0)
            {
                ft_echo(a->cmds);
                exit (0);
            }
            gl_fd[0] = 0;
        }
        else if (strncmp(a->cmds[i].argv[0], "pwd\0", 4) == 0)
        {
            pid[i] = fork();
            forking(pid, i, &pfd, a);
            if (pid[i] == 0)
            {
                ft_pwd(a->cmds);
                exit (gl_fd[0]);
            }
        }
        else if (strncmp(a->cmds[i].argv[0], "env\0", 4) == 0)
        {
            pid[i] = fork();
            forking(pid, i, &pfd, a);
            if (pid[i] == 0)
            {
                ft_env(a->cmds, a->envp);
                exit (0);
            }
        }
        else if (strncmp(a->cmds[i].argv[0], "export\0", 7) == 0)
        {
            pid[i] = fork();
            forking(pid, i, &pfd, a);
            if (pid[i] == 0)
            {
                if (a->cmds[i].argv[1] == NULL)
					ft_export(a->cmds, a->envp, a->exp);
			    else
				ft_export_arg(a->cmds, a->envp, a->exp);
                exit (gl_fd[0]);
            }
		}
        else if (strncmp(a->cmds[i].argv[0], "unset\0", 6) == 0)
            ft_unset(a->cmds, a->envp, a->exp);
        else if (strncmp(a->cmds[i].argv[0], "exit\0", 6) == 0)
            ft_exit(a->cmds);
        ///ВЫПОЛНЕНИЕ БИНАРНЫХ КОМАНД
        else
        {
            char *path = search_path(a, &a->cmds[i], a->envp);
            pid[i] = fork();
            if (pid[i] != 0)
            {
                if (pfd != NULL && i < a->cmds_num  -1)
                    close(pfd[i][1]);
            }
            if (pid[i] == 0)
            {
                if (a->cmds_num > 1)
                    dup_fd(pfd, i, a->cmds_num, a->cmds);
                else
                {
                    dup2(a->cmds[i].fd[0], 0);
                    dup2(a->cmds[i].fd[1], 1);
                }
                if (path == NULL)
                    exit(127);
                
                exec_command(a, &a->cmds[i], a->envp, path);
            }
            signal(SIGINT, ctrl_c);
            signal(SIGQUIT, ctrl_slash);
        }
    }
        j = -1;
        while (++j < a->cmds_num)
		{
			waitpid(pid[j], &status[j], 0);
			f = WSTOPSIG(status[j]);
			if (gl_fd[0] == 0 && f != 0)
				gl_fd[0] = errno;
		}
        printf("code : %d\n", gl_fd[0]);
        // printf("code : %d\n", gl_fd[1]);
}