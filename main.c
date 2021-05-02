#include "includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_all	*all;
	int		len;
	char	str[2000];

	all = init_all(envp);
	shlvl_ini(all);
	while (1)
	{
		new_line(all, str);
		nocanon(all);
		read_line(all, str);
		if (!ft_strncmp(str, "\n", 2))
		{
			canon(all);
			parser(all);
			save_history(all);
		}
	}
	ft_putchar_fd('\n', 1);
	return (0);
}

// int main(int ac, char **av, char **envp)
//  {
//  	t_all *all;
//  	char str[2000];
//  	all = init_all(envp);
//  	shlvl_ini(all);
//  	new_line(all, str);
//  	all->pos--;
//  	all->hist_len--;
//  	parser(all);
//  	while(1);
//  }