#include "includes/minishell.h"

void	ctrl_slash(int sig)
{
	if (g_res[0] == 0)
	{
		write(1, "Quit: ", 6);
		ft_putnbr_fd(sig, 1);
		write(1, "\n", 1);
		g_res[0] = 131;
	}
}

void	ctrl_c(int sig)
{
	if (g_res[1] == 0)
	{
		write(1, "\n", 1);
		g_res[0] = 130;
	}
}
