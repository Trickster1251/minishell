#include "includes/minishell.h"

void    ctrl_slash(int sig)
{
    if (gl_fd[0] == 0)
    {
        write(1, "Quit: ", 6);
        ft_putnbr_fd(sig, 1);
        write(1, "\n", 1);
        gl_fd[0] = 131;
    }
}

void    ctrl_c(int sig)
{
    if (gl_fd[1] == 0)
    {
        write(1, "\n", 1);
        gl_fd[0] = 130;
    }
}

int     dup_fd(int **pfd, int i, int cmd_com, t_cmd *cmd)
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
    return (0);
}

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