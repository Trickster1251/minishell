#include "includes/minishell.h"

void	ctrl_slash(int sig)
{
	if (gl_fd[0] == 0)
	{
		write(1, "Quit: ", 6);
		ft_putnbr_fd(sig, 1);
		write(1, "\n", 1);
		gl_fd[0] = 131;
	}
}

void	ctrl_c(int sig)
{
	if (gl_fd[1] == 0)
	{
		write(1, "\n", 1);
		gl_fd[0] = 130;
	}
}
