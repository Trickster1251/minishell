#include "includes/minishell.h"

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

int		dup_fd(int **pfd, int i, int cmd_com, t_cmd *cmd)
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
		close(pfd[i - 1][1]);
		dup2(pfd[i - 1][0], 0);
		close(pfd[i - 1][1]);
	}
	if (cmd[i].fd[0] != 0)
		dup2(cmd[i].fd[0], 0);
	if (cmd[i].fd[1] != 1)
		dup2(cmd[i].fd[1], 1);
	return(0);
}

int	count_pipes(char *s, t_cmd *cmd)
{
	int		i;
	int		count;

	i = -1;
	count = 0;
	while (s[++i])
	{
		if (s[i] == '|')
			count++;
	}
	return (count);
}
