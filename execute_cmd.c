#include "includes/minishell.h"

char	*search_path(t_all *all, t_cmd *cmd, t_list *envp)
{
	int			i;
	struct stat	buf;
	char		**env;
	char		**path;
	char		*tmp;

	tmp = search_key(envp, "PATH");
	path = ft_split(tmp, ':');
	if (tmp)
		free(tmp);
	if (!path)
		path = ft_calloc(1, sizeof(char *));
	i = -1;
	while (path[++i] && cmd->fd[0] != -1 && cmd->fd[1] != -1)
	{
		tmp = is_binary(i, cmd, path);
		if (tmp != NULL)
			return (tmp);
	}
	free_str_arr(path);
	if (lstat(cmd->argv[0], &buf) == 0)
		return (found_binary(cmd));
	printf("minishell: %s: command not found\n", cmd->argv[0]);
	g_res[0] = 127;
	return (NULL);
}

void	init_values(t_all *a, int i)
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
	int	status;

	i = -1;
	while (++i < a->cmds_num)
	{
		waitpid(pid[i], &status, 0);
		f = WSTOPSIG(status);
		if (g_res[0] != 127 && f != 0)
			g_res[0] = 2;
	}
}

void	execute_cmd(t_all *a)
{
	int		i;
	int		**pfd;
	pid_t	*pid;
	int		com;

	i = -1;
	pfd = 0;
	pid = calloc(sizeof(pid_t), a->cmds_num);
	a->exp = a->envp;
	if (a->cmds_num > 1)
		pfd = pipes_fd(a);
	while (++i < a->cmds_num)
	{
		init_values(a, i);
		com = is_builtin(a, i, pid, pfd);
		if (!com)
			exec_command(a, i, pid, pfd);
		signal(SIGINT, ctrl_c);
		signal(SIGQUIT, ctrl_slash);
	}
	wait_pid(a, pid);
	free(pid);
	free_pfd(a, &pfd);
}
