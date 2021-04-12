#include "includes/minishell.h"
#include <string.h>


t_list *parse_env(char **env)
{
	t_list *lst;
	t_env *tmp;
	char *tmp_str;

	lst = NULL;

	
	while (*env)
	{
		tmp_str = *env;
		while (*tmp_str)
		{
			if (*tmp_str == '=')
			{
				tmp = (t_env*)malloc(sizeof(t_env));
				if (!tmp)
					return (NULL);
				tmp->key = ft_substr(*env, 0, tmp_str - *env);
				tmp->value = ft_strdup(tmp_str + 1);
				ft_lstadd_back(&lst, ft_lstnew(tmp));
				break;
			}
			tmp_str++;
		}
		env++;
	}
	return (lst);
}

t_all *init_all(char **env)
{
	t_all *all;

	all = (t_all*)malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	all->hist_list = NULL;
	all->history = NULL;
	all->hist_len = 0;
	all->env = parse_env(env);
	return (all);
}

int		ft_putint(int c)
{
	write(1, &c, 1);
	return (c);
}

char 	*get_env_val(t_list *env, char *key)
{
	t_env *tmp;

	while (env)
	{
		tmp = env->content;
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1 ))
			return (tmp->value);
		env = env->next;
	}
	return (NULL);
}


int		get_hist_array(t_all *all)
{
	t_list *tmp;
	int i;

	all->hist_len = ft_lstsize(all->hist_list);
	i = 0;
	all->history = (char**)malloc(sizeof(char*) * (all->hist_len + 1));
	if (!all->history)
		return (-1);
	tmp = all->hist_list;
	while(tmp)
	{
		all->history[i] = tmp->content;
		i++;
		tmp = tmp->next;
	}
	all->history[i] = NULL;
 	return (0);
}

int		read_history(t_all *all)
{
	int fd;
	int bytes;
	char *line;
	int i;

	line = NULL;
	fd = open(".minishell_history", O_RDWR | O_CREAT, 0777);
	ft_lstclear(&all->hist_list, free);
	i = 0;
	if (all->history)
		free(all->history);
	all->hist_list = NULL;
	while ((bytes = get_next_line(fd, &line)) != 0)
	{
		if (line[0] != '\0' && line[0] != '\04')
			ft_lstadd_back(&all->hist_list, ft_lstnew(line));
		else
			free(line);
		if (all->hist_list == NULL)
		{	
			close(fd);
			return (-1);
		}
		line = NULL;
	}
	close(fd);
	if (line)
	{
		if (line[0] != '\0' && line[0] != '\04')
			ft_lstadd_back(&all->hist_list, ft_lstnew(line));
		else
			free(line);
	}
	t_list *tmp = all->hist_list;
	if (fd < 0 || bytes < 0)
		return (-1);
	return (0);
}

int		save_history(t_all *all)
{
	int fd;
	t_list *tmp;

	fd = open(".minishell_history", O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (fd < 0)
	{
		close (fd);
		return (-1);
	}
	tmp = all->hist_list;
	while(tmp)
	{
		if (ft_strlen(tmp->content) > 0)
			ft_putendl_fd(tmp->content, fd);
		tmp = tmp->next;
	}
	close(fd);
	return (0);
}

int		hist_strjoin(t_all *all, char *str)
{
	t_list *tmp;
	int i;
	char *t;

	i = 0;
	tmp = ft_lstlast(all->hist_list);
	if (str[0] == '\04')
		return (0);
	if (ft_strncmp(all->history[all->pos], all->history[all->hist_len - 1], ft_strlen(all->history[all->pos])))
	{
		t = all->history[all->hist_len - 1];
		all->history[all->hist_len - 1] = ft_strjoin(all->history[all->pos], str);
		free(t);
	}
	else
		all->history[all->hist_len - 1] = my_strjoin(all->history[all->hist_len - 1], str);
	tmp->content = all->history[all->hist_len - 1];
	return (0);
}

void	print_previus(t_all *all)
{
	all->pos--;
	if (all->pos < 0)
	{
		all->pos = 0;
		if (all->hist_len == 1)
			return ;
	}
	ft_putstr_fd(all->history[all->pos], 1);
}

void	print_next(t_all *all)
{
	all->pos++;
	if (all->pos > all->hist_len - 1)
		all->pos = all->hist_len - 1;
	ft_putstr_fd(all->history[all->pos], 1);
}

void	nocanon(t_all *all)
{
	all->term_name = get_env_val(all->env, "TERM");
	tcgetattr(0, &all->term);
	all->term.c_lflag &= ~(ECHO);
	all->term.c_lflag &= ~(ICANON);
	tcsetattr(0, TCSANOW, &all->term);
	tgetent(0, all->term_name);
	tputs("minishell$ ", 1, ft_putint);
	tputs(save_cursor, 1, ft_putint);
}

int		new_line(t_all *all, char *str)
{
	t_list	*tmp;

	if (read_history(all) < 0)
		return (-1);
	tmp = ft_lstlast(all->hist_list);
	if (all->hist_len == 0 || ft_strlen(tmp->content) != 0)
		ft_lstadd_back(&all->hist_list, ft_lstnew(ft_strdup("")));
	if (get_hist_array(all) < 0)
		return (-1);
	all->pos = all->hist_len - 1;
	ft_bzero(str, 2000);
	return (0);
}

// int		parser(t_all *all)
// {

// }

int		main(int ac, char **av, char **env)
{
	t_all *all;
	int len;
	char str[2000];

	all = init_all(env);
	while(str[0] != '\04') 
	{
		new_line(all, str);
		nocanon(all);
		while (ft_strncmp(str, "\n", 2) && str[0] != '\04')
		{
			len = read(0, str, 1000);
			str[len] = 0;
			if (!ft_strncmp(str, "\e[A", 5))
			{
				if (all->hist_len > 1)
				{
					tputs(restore_cursor, 1, ft_putint);
					print_previus(all);
					tputs(tigetstr("ed"), 1, ft_putint);
				}
			}
			else if (!ft_strncmp(str, "\e[B", 5))
			{
				tputs(restore_cursor, 1, ft_putint);
				print_next(all);
				tputs(tigetstr("ed"), 1, ft_putint);
			}
			else if (!ft_strncmp(str, "\177", 5))
			{
				if (ft_strlen(all->history[all->pos]) > 0)
				{
					tputs(cursor_left, 1, ft_putint);
					all->history[all->pos][ft_strlen(all->history[all->pos]) - 1] = '\0';
					tputs(tgetstr("dc", 0), 1, ft_putint);
				}
			}
			else if (!ft_strncmp(str, "\e[D", 5))
			{
				if (ft_strlen(all->history[all->pos]) > 0)
					tputs(cursor_left, 1, ft_putint);
			}
			else if (!ft_strncmp(str, "\e[C", 5))
				tputs(cursor_right, 1, ft_putint);
			else
			{
				if (hist_strjoin(all, str) < 0)
					return (-1);
				write(1, str, len);
			}
		}
		
		save_history(all);
	}
	all->term.c_lflag |= ECHO;
	all->term.c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, &all->term);
	write(1, "\n", 1);
	return (0);
}