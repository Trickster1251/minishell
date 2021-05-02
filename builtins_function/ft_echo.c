#include "../includes/minishell.h"

void	ft_echo(t_cmd *cmd)
{
	int	i;
	int	echo_flag;

	i = 0;
	echo_flag = 0;
	if (len_arr(cmd->argv) != 1)
	{
		if ((ft_strncmp(cmd->argv[1], "-n\0", 3) == 0))
		{
			echo_flag = 1;
			i++;
		}
		while (cmd->argv[++i])
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
	gl_fd[0] = 0;
}
