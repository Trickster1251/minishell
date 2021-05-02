#include "./includes/minishell.h"

void	print_previus(t_all *all)
{
	all->pos--;
	if (all->pos < 0)
	{
		all->pos = 0;
		if (all->hist_len == 1)
			return ;
	}
	ft_putstr_fd(all->hist[all->pos], 1);
}

void	print_next(t_all *all)
{
	all->pos++;
	if (all->pos > all->hist_len - 1)
		all->pos = all->hist_len - 1;
	ft_putstr_fd(all->hist[all->pos], 1);
}

int	print_merror(t_all *all)
{
	printf("minishell: memmory allocate error\n");
	return (-1);
}
