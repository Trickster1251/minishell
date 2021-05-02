#include "../includes/minishell.h"

int		new_line(t_all *all, char *str)
{
	t_list	*tmp;

	read_history(all);
	tmp = ft_lstlast(all->hist_list);
	if ((all->hist_len == 0) || tmp == NULL)
		ft_lstadd_back(&all->hist_list, ft_lstnew(ft_strdup("")));
	else if (all->hist_len == 0 || ft_strlen(tmp->content) != 0)
		ft_lstadd_back(&all->hist_list, ft_lstnew(ft_strdup("")));
	if (get_hist_array(all) < 0)
		return (-1);
	all->pos = all->hist_len - 1;
	ft_bzero(str, 2000);
	return (0);
}

int read_line(t_all *all, char *str)
{
	int len;

	while (ft_strncmp(str, "\n", 2))
	{
		len = read(0, str, 1000);
		str[len] = 0;
		if (!ft_strncmp(str, "\e[A", 5))
			up_arrow(all);
		else if (!ft_strncmp(str, "\e[B", 5))
			down_arrow(all);
		else if (!ft_strncmp(str, "\177", 5))
			backspace_key(all);
		else if (!ft_strncmp(str, "\e[D", 5))
			continue;
		else if (!ft_strncmp(str, "\e[C", 5))
			continue;
		else if (!ft_strncmp(str, "\4", 2) && ft_strlen(all->hist[all->pos]))
			continue;
		else if (!ft_strncmp(str, "\4", 2) && !ft_strlen(all->hist[all->pos]))
			ctrl_d_term(all);
		else if (!ft_strncmp(str, "\03", 2))
		{
			canon(all);
			write(1, "\n", 1);
			break;
		}
		else
		{
			if (hist_strjoin(all, str) < 0)
				return (-1);
			write(1, str, len);
		}
	}
	return (0);
}