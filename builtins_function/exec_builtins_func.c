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
	gl_fd[0] = 0;
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

void	exec_export(t_all *a, pid_t *pid, int i, int **pfd)
{
	if (a->cmds[i].argv[1] == NULL)
	{
		pid[i] = fork();
		forking(pid, i, &pfd, a);
		if (pid[i] == 0)
			print_export(&a->exp);
	}
	else
		ft_export_arg(a->cmds, a->envp, a->exp);
}
