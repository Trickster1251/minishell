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

void	free_str_arr(char **arr)
{
	int i;

	i = 0;
	while(arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char     *search_path(t_all *all, t_cmd *cmd, t_list *envp)
{
    int     i;
    struct stat buf;
    char    **env;
    char    **path;
    char    *comd;
	char *tmp;

	tmp = NULL;
	tmp = search_key(envp, "PATH");
    path = ft_split(tmp, ':');
	if (tmp)
		free(tmp);
	if (!path)
		path = ft_calloc(1, sizeof(char*));
    comd = ft_strjoin("/", cmd->argv[0]);
    i = -1;
    gl_fd[0] = 0;

    int     fd;

    while(path[++i] && cmd->fd[0] != -1 && cmd->fd[1] != -1)
    {
		tmp = ft_strjoin(path[i], comd);
        if (lstat(tmp, &buf) == 0)
        {
			free(tmp);
            tmp = ft_strjoin(path[i], comd);
			free_str_arr(path);
			free(comd);
            return (tmp);
        }
		free(tmp);
    }
	free_str_arr(path);
	free(comd);
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
    struct stat buf;
	int i;
	
	i = 0;
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
	int     **pfd;

	pfd = *fd;
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
}

// void    forking(pid_t *pid, int i, int ***fd, t_all *a)
// {
//     int     **pfd = *fd;
//     if (pid[i] != 0)
//     {
//         if (pfd != NULL && i < a->cmds_num - 1)
//         {
//             dup2(a->cmds[i].fd[1], 1);
//             close(pfd[i][1]);
//         }
//     }
//     else if (pid[i] == 0)
//     {
//         if (a->cmds_num > 1)
//             dup_fd(pfd, i, a->cmds_num, a->cmds);
//         else
//         {
//             dup2(a->cmds[i].fd[0], 0);
//             dup2(a->cmds[i].fd[1], 1);
//         }
//     }
//     return ;
// }

////////////
////////////
////////////

void	exec_echo(t_all *a, pid_t *pid, int i, int **pfd)
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

void	exec_env(t_all *a, pid_t *pid, int i, int **pfd)
{
	pid[i] = fork();
	forking(pid, i, &pfd, a);
	if (pid[i] == 0)
	{
		if (a->cmds[i].argv[1] == NULL)
			print_env(a->envp);
		else
		{
			printf("env: %s: No such file or directory\n", a->cmds[i].argv[1]);
			gl_fd[0] = 1;
		}
		exit(0);
	}
}

void    init_values(t_all *a, int i)
{
	int     j;

	j = -1;
	a->cmds[i].fd[0] = 0;
	a->cmds[i].fd[1] = 1;
	a->cmds[i].count_redir = 0;
	a->cmds[i].count_redir = count_redir(a->cmds[i].argv);
	if (a->cmds[i].count_redir != 0)
		create_open_fd(a, &a->cmds[i], a->cmds[i].argv);
	while (a->cmds[i].argv[++j])
		unshield(a->cmds[i].argv[j]);
}

void	exec_pwd(t_all *a, pid_t *pid, int i, int **pfd)
{
	pid[i] = fork();
	forking(pid, i, &pfd, a);
	if (pid[i] == 0)
	{
		ft_pwd(a->cmds);
		exit(gl_fd[0]);
	}
}

void	exec_export(t_all *a, pid_t *pid, int i, int **pfd)
{
	if (a->cmds[i].argv[1] == NULL)
	{
		pid[i] = fork();
		forking(pid, i, &pfd, a);
		if (pid[i] == 0)
        {
			print_export(&a->exp);
            exit(0);
        }
	}
	else
		ft_export_arg(a->cmds, a->envp, a->exp);
}

void     execute_cmd(t_all *a)
{
    int     i;
    int     **pfd;
	int     f;
    pid_t   pid[a->cmds_num];
	int     status[a->cmds_num];
	char *path;

	int j;
    i = -1;
    pfd = 0;
    a->exp = a->envp;
    if (a->cmds_num > 1)
        pfd = pipes_fd(a);
    while(++i < a->cmds_num)
    {
        init_values(a, i);    

        if (strncmp(a->cmds[i].argv[0], "cd\0", 3) == 0)
            ft_cd (&a->cmds[i], a->envp, a->exp);
        else if (strncmp(a->cmds[i].argv[0], "echo\0", 5) == 0)
            exec_echo(a, pid, i, pfd);
        else if (strncmp(a->cmds[i].argv[0], "pwd\0", 4) == 0)
            exec_pwd(a, pid, i, pfd);
        else if (strncmp(a->cmds[i].argv[0], "env\0", 4) == 0)
            exec_env(a, pid, i, pfd);
        else if (strncmp(a->cmds[i].argv[0], "export\0", 7) == 0)
            exec_export(a, pid, i, pfd);
		else if (strncmp(a->cmds[i].argv[0], "unset\0", 6) == 0)
			ft_unset(a->cmds, a->envp, a->exp);
		else if (strncmp(a->cmds[i].argv[0], "exit\0", 6) == 0)
			ft_exit(a->cmds);
		else
		{
			path = search_path(a, &a->cmds[i], a->envp);
			pid[i] = fork();
            forking(pid, i, &pfd, a);
            if (pid[i] == 0)
            {
				if (path == NULL )
					exit(127);
				else
				{
                    char    **env = lst_to_array(a->envp);
                    execve(path, a->cmds[i].argv, env);
				}
            }
			if (path && ft_strncmp(a->cmds[i].argv[0], "./minishell\0", 12))
                free(path);
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
		{
			gl_fd[0] = errno;
		}
	}
	printf("code : %d\n", gl_fd[0]);
}