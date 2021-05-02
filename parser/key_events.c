#include "../includes/minishell.h"

void	up_arrow(t_all *all)
{
	if (all->hist_len > 1)
	{
		tputs(restore_cursor, 1, ft_putint);
		print_previus(all);
		tputs(tigetstr("ed"), 1, ft_putint);
	}
}

void	down_arrow(t_all *all)
{
	tputs(restore_cursor, 1, ft_putint);
	print_next(all);
	tputs(tigetstr("ed"), 1, ft_putint);
}

void	backspace_key(t_all *all)
{
	if (ft_strlen(all->hist[all->pos]) > 0)
	{
		tputs(cursor_left, 1, ft_putint);
		all->hist[all->pos][ft_strlen(all->hist[all->pos]) - 1] = '\0';
		tputs(tgetstr("dc", 0), 1, ft_putint);
	}
}

void	ctrl_d_term(t_all *all)
{
	canon(all);
	save_history(all);
	printf("exit\n");
	exit(0);
}

int	ft_putint(int c)
{
	write(1, &c, 1);
	return (c);
}
