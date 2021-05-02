#include "includes/minishell.h"

char	*absolute_path(t_cmd *cmd)
{
	int	fd;

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
		cmd->argv[0] = absolute_path(cmd);
    printf("minishell: %s: command not found\n", cmd->argv[0]);
    gl_fd[0] = 127;
    return (NULL);
}

void    init_values(t_all *a, int i)
{
	int	j;

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

void	exec_command(t_all *a, int i, pid_t *pid, int **pfd)
{
	char	*path;
	char	**env;

	path = search_path(a, &a->cmds[i], a->envp);
	pid[i] = fork();
	forking(pid, i, &pfd, a);
	if (pid[i] == 0)
	{
		if (path == NULL)
			exit(127);
		else
		{
			env = lst_to_array(a->envp);
			execve(path, a->cmds[i].argv, env);
		}
	}
	if (path && ft_strncmp(a->cmds[i].argv[0], "./minishell\0", 12))
		free(path);
}

void	wait_pid(t_all *a, pid_t *pid)
{
	int	f;
	int	i;
	int status;

	i = -1;
	while (++i < a->cmds_num)
	{
		waitpid(pid[i], &status, 0);
		f = WSTOPSIG(status);
		if (gl_fd[0] == 0 && f != 0)
			gl_fd[0] = 2;
	}
}

void	execute_cmd(t_all *a)
{
	int	i;
	int	**pfd;
	pid_t	*pid;

	i = -1;
	pfd = 0;
	pid = calloc(sizeof(pid_t), a->cmds_num);
	a->exp = a->envp;
	if (a->cmds_num > 1)
		pfd = pipes_fd(a);
	while (++i < a->cmds_num)
	{
		init_values(a, i);
		if (is_builtin(a, i, pid, pfd));
		else
			exec_command(a, i, pid, pfd);
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, ctrl_slash);
	}
	wait_pid(a, pid);
    free(pid);
    printf("code : %d\n", gl_fd[0]);
}
