#include "../includes/minishell.h"

void	exec_echo(t_all *a, pid_t *pid, int i, int **pfd)
{
	pid[i] = fork();
	forking(pid, i, &pfd, a);
	if (pid[i] == 0)
	{
		ft_echo(a->cmds);
		exit (0);
	}
	g_res[0] = 0;
}

void	exec_pwd(t_all *a, pid_t *pid, int i, int **pfd)
{
	pid[i] = fork();
	forking(pid, i, &pfd, a);
	if (pid[i] == 0)
	{
		ft_pwd(a->cmds);
		exit(g_res[0]);
	}
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
			g_res[0] = 1;
		}
		exit(0);
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
		ft_export_arg(a->cmds, a);
}

int	is_builtin(t_all *a, int i, pid_t *pid, int **pfd)
{
	if (strncmp(a->cmds[i].argv[0], "cd\0", 3) == 0)
		ft_cd(&a->cmds[i], a->envp, a->exp);
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
		return (0);
	return (1);
}
