#include "includes/minishell.h"

// Готово

void    ft_echo(t_cmd *cmd)
{
    int i = 0;
    int echo_flag = 0;

    if (len_arr(cmd->argv) != 1)
    {
        if (!ft_strncmp(cmd->argv[1], "-n\0", 3))
        {
            echo_flag = 1;
            i = 1;
        }
        while(cmd->argv[++i])
        {
            if (!ft_strncmp(cmd->argv[i], "$?\0", 3))
                ft_putnbr_fd(gl_fd[0], cmd->fd[1]);
            else
                write(cmd->fd[1], cmd->argv[i], ft_strlen(cmd->argv[i]));
            if (i != len_arr(cmd->argv) - 1)
                write(cmd->fd[1], " ", 1);
        }
    }
    if (echo_flag == 0)
        write(cmd->fd[1], "\n", 1);
}