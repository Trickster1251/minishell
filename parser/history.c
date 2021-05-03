#include "../includes/minishell.h"

void	read_loop(t_all *all, char *line, int fd)
{
	if (line[0] != '\0' && line[0] != '\04')
		ft_lstadd_back(&all->hist_list, ft_lstnew(line));
	else
		free(line);
	if (all->hist_list == NULL)
	{
		print_merror(all);
		close(fd);
		exit(EXIT_FAILURE);
	}
	line = NULL;
}

int	read_history(t_all *all)
{
	int		fd;
	char	*line;
	int		i;

	line = NULL;
	fd = open(all->file, O_RDWR | O_CREAT, 0777);
	ft_lstclear(&all->hist_list, free);
	i = 0;
	if (all->hist)
		free(all->hist);
	all->hist_list = NULL;
	while (get_next_line(fd, &line) != 0)
		read_loop(all, line, fd);
	close(fd);
	if (line)
	{
		if (line[0] != '\0' && line[0] != '\04')
			ft_lstadd_back(&all->hist_list, ft_lstnew(line));
		else
			free(line);
	}
	if (fd < 0)
		return (-1);
	return (0);
}

int	save_history(t_all *all)
{
	int		fd;
	t_list	*tmp;

	fd = open(all->file, O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (fd < 0)
	{
		close (fd);
		return (-1);
	}
	tmp = all->hist_list;
	while (tmp)
	{
		if (ft_strlen(tmp->content) > 0)
			ft_putendl_fd(tmp->content, fd);
		tmp = tmp->next;
	}
	close(fd);
	return (0);
}

int	hist_strjoin(t_all *all, char *str, int len)
{
	t_list	*tmp;
	char	*t;

	tmp = ft_lstlast(all->hist_list);
	if (str[0] == '\04')
		return (0);
	if (ft_strncmp(all->hist[all->pos], all->hist[all->hist_len - 1],
			ft_strlen(all->hist[all->pos])))
	{
		t = all->hist[all->hist_len - 1];
		all->hist[all->hist_len - 1] = ft_strjoin(all->hist[all->pos], str);
		if (all->hist[all->hist_len - 1] == NULL)
			return (print_merror(all));
		free(t);
	}
	else
	{
		all->hist[all->hist_len - 1] = my_strjoin
			(all->hist[all->hist_len - 1], str);
		if (all->hist[all->hist_len - 1] == NULL)
			return (print_merror(all));
	}
	tmp->content = all->hist[all->hist_len - 1];
	write(1, str, 1);
	return (0);
}

int	get_hist_array(t_all *all)
{
	t_list	*tmp;
	int		i;

	all->hist_len = ft_lstsize(all->hist_list);
	i = 0;
	all->hist = (char **)malloc(sizeof(char *) * (all->hist_len + 1));
	if (!all->hist)
		return (-1);
	tmp = all->hist_list;
	while (tmp)
	{
		all->hist[i] = tmp->content;
		i++;
		tmp = tmp->next;
	}
	all->hist[i] = NULL;
	return (0);
}
