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

// char	get_next_char(t_line *src)
// {
// 	if (!src->str)
// 		return (-1);
// 	if (src->pos < src->len)
// 		src->pos++;
// 	return (src->str[src->pos]);
// }

char	previus_char(t_line *src)
{
	char c;
	int i;

	if (src->pos != 0)
		i = src->pos - 1;
	else
		i = src->pos;
	return (src->str[i]);
}

int		shield(t_all *all, t_line *src)
{
	int i;
	int k;
	int redir;
	int rev_redir;

	i = 0;
	k = 0;
	redir = 0;
	rev_redir = 0;
	while (src->str[src->pos])
	{
		if (src->str[src->pos] == '"' && all->val.in_dqt == 0 && all->val.in_qt == 0 && previus_char(src) != '\\')
			all->val.in_dqt = 1;
		else if (src->str[src->pos] == '\'' && all->val.in_qt == 0 && previus_char(src) != '\\')
			all->val.in_qt = 1;
		else if (src->str[src->pos] == '"' && all->val.in_dqt == 1 && all->val.in_qt == 0 && previus_char(src) != '\\')
			all->val.in_dqt = 0;
		else if (src->str[src->pos] == '\'' && all->val.in_qt == 1 && previus_char(src) != '\\')
			all->val.in_qt = 0;
		else if (src->str[src->pos] == '$' && (all->val.in_qt == 1 || previus_char(src) == '\\'))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '#' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '>' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
				src->str[src->pos] *= -1;
		else if (src->str[src->pos] == ';' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '<' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '&' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == ' ' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '(' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == ')' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '|' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1)) //добавить "'$PATH'"
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '"' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '\\' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		else if (src->str[src->pos] == '\'' && (previus_char(src) == '\\' || all->val.in_dqt == 1))
			src->str[src->pos] *= -1;
		if ((src->str[src->pos] == '|' || src->str[src->pos] == '>' || src->str[src->pos] == '<')
		&& src->pos == src->len - 2)
		{
			printf("minishell: syntax error near unexpected token `newline'\n");
			return (-1);
		}
		if (src->str[src->pos] == '|' && src->pos == 0)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (-1);
		}
		if (src->str[src->pos] == '&')
		{
			printf("minishell: syntax error unexpected token `&'\n");
			return (-1);
		}
		if (src->str[src->pos] == '|')
		{
			k++;
			if (k > 1)
			{
				printf("minishell: syntax error near unexpected token `|'\n");
				return (-1);
			}
		}
		else
			k = 0;
		if (src->str[src->pos] == '>')
		{
			redir++;
			if (redir > 2)
			{
				printf("minishell: syntax error near unexpected token `>'\n");
				return (-1);
			}
			if (src->str[src->pos + 1] == '>' && previus_char(src) == '<')
			{
				printf("minishell: syntax error near unexpected token `<'\n");
				return (-1);
			}
			if (src->str[src->pos + 1] == '<')
			{
				printf("minishell: syntax error near unexpected token `<'\n");
				return (-1);
			}
		}
		else
			redir = 0;
		if (src->str[src->pos] == '<')
		{
			rev_redir++;
			if (rev_redir > 2)
			{
				printf("minishell: syntax error near unexpected token `<'\n");
				return (-1);
			}
			
			if (src->str[src->pos + 1] == '>' && previus_char(src) == '<')
			{
				printf("minishell: syntax error near unexpected token `<'\n");
				return (-1);
			}
		}
		else
			rev_redir = 0;
		src->pos++; 
	}
	if (all->val.in_dqt == 1 || all->val.in_qt == 1)
	{
		printf("minishell: quote is not closed\n");
		return (-1);
	}
	return (0);
}

// int		line_vaidator(t_all *all, t_line *src)
// {
// 	int i;


// }
int 	unshield(char *str)
{
	int i;
	
	i = 0;
	while (str[i])
	{
		if (str[i] < 0)
			str[i] *= -1;
		i++;
	}
	return (0);
}

char		*ft_charjoin(char const *s1, char c)
{
	int		len;
	int		i;
	char	*a;

	if (!s1)
		return (NULL);
	len = ft_strlen((char *)s1) + 1;
	a = (char*)malloc((len + 1) * sizeof(char));
	i = 0;
	if (a == NULL)
		return (NULL);
	while (s1[i])
	{
		a[i] = s1[i];
		i++;
	}
	a[i] = c;
	a[i + 1] = '\0';
	free ((char *)s1);
	return (a);
}

int		remove_ch(t_line *src)
{
	char *tmp;

	tmp = ft_strdup("");
	if (!tmp)
		return (-1);
	src->pos = 0;
	while (src->str[src->pos])
	{
		if (src->str[src->pos] != '\\' && src->str[src->pos] != '"' && src->str[src->pos] != '\'' && src->str[src->pos] != '\n')
		{
			tmp = ft_charjoin(tmp, src->str[src->pos]);
			if (!tmp)
				return (-1);
		}
		src->pos++;
	}
	free(src->str);
	src->str = tmp;
	return (0);
}

// int		simple_cmd(t_all *all, char *str)
// {
// 	int num;
// 	int i;
// 	int pipe_num;
// 	int redir_num;
// 	int len;

// 	num = 0;
// 	i = 0;
// 	pipe_num = 0;
// 	len = ft_strlen(str);
// 	all->cmds = (t_command*)malloc(sizeof(t_command));
// 	all->cmds->args = (char**)malloc(sizeof(char*) * 1);
// 	if (!all->cmds || !all->cmds->args)
// 		return (-1);
// 	while (str[i])
// 	{

// 	}
// }

int		make_cmd(t_all *all)
{
	char **cmds;
	char **childs;
	int i;
	int j;
	char *d_pointer;
	char *tmp1;
	char *tmp2;

	i = 0;
	d_pointer = NULL;
	cmds = ft_split(all->src->str, ';');
	while(cmds[i])
	{
		if ((d_pointer = ft_strchr(cmds[i], '$')))
		{
			tmp1 = ft_substr(cmds[i], 0, d_pointer - cmds[i]);
			tmp2 = get_env_val(all->env, d_pointer + 1);
			tmp1 = my_strjoin(tmp1, tmp2);
			free(cmds[i]);
			cmds[i] = tmp1;
		}
		unshield(cmds[i]);
		printf("%s\n", cmds[i]);
		i++;
	}
	// free(cmds[0]);
	// free(cmds);               для теста
	//printf("\n");
	return (0);
}

int		parser(t_all *all)
{
	t_line src;
	
	ft_bzero(&all->val, sizeof(all->val));
	src.str = ft_strdup(all->history[all->hist_len - 1]);
	src.len = ft_strlen(src.str);
	src.pos = 0;
	int ret = shield(all, &src);
	ret = remove_ch(&src);
	//Символы удалены, далее подставить переменные
	//Почему-то не все работает в экранировании >>
	all->src = &src;
	make_cmd(all);
	free(src.str);
	// unshield(&src);
	// printf("%s\n", src.str);
	return (ret);
}	

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
		parser(all);
		save_history(all);
	}
	all->term.c_lflag |= ECHO;
	all->term.c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, &all->term);
	write(1, "\n", 1);
	return (0);
}