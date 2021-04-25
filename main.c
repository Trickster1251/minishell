#include "includes/minishell.h"
#include <string.h>

t_all *init_all(char **envp)
{
	t_all *all;

	all = (t_all*)malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	all->hist_list = NULL;
	all->history = NULL;
	all->hist_len = 0;
	all->envp = parse_env(envp);
	all->cmds = NULL;
	all->src = NULL;
	return (all);
}

int		ft_putint(int c)
{
	write(1, &c, 1);
	return (c);
}

char 	*get_env_val(t_list *envp, char *key)
{
	t_env *tmp;

	while (envp)
	{
		tmp = envp->content;
		if (!ft_strncmp(tmp->key, key, ft_strlen(key) + 1 ))
			return (tmp->value);
		envp = envp->next;
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
	all->term_name = get_env_val(all->envp, "TERM");
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
		else if (src->str[src->pos] == ' ' && (all->val.in_qt == 1 || previus_char(src) == '\\' || all->val.in_dqt == 1))
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
		if (src->str[src->pos] == '\'' && (previus_char(src) == '\\' || all->val.in_dqt == 1))
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

char *end_var(char *s)
{
	char *s1;
	char *tmp;

	s1 = s;
	while(*s1 && (ft_isalnum(*s1) || *s1 == '_'))
		s1++;
	tmp = ft_substr(s, 0, s1 - s);
	return (tmp);
}

char 	*after_var(char *s)
{
	while(*s && (ft_isalnum(*s) || *s == '_'))
		s++;
	return (s);
}

int		print_merror(t_all *all)
{
	printf("minishell: memmory allocate error\n");
	return (-1);
}

int		argv_len(char **argv)
{
	int i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

int		check_argv(t_all *all, char **argv)
{
	int i;
	int j;
	int k;
	char c;

	i = 0;
	j = 0;
	while (argv[i])
	{
		k = 0;
		j = 0;
		while (argv[i][j])
		{
			if ((0 == k || 3 == k) && (argv[i][j] == '<' || argv[i][j] == '>'))
			{
				c = argv[i][j];
				k = 1;
			}
			if (argv[i][j] == ' ' && k == 0)
				k = 2;
			if (ft_isalnum(argv[i][j]) || argv[i][j] == '_' || '$' == argv[i][j])
				k = 3;
			j++;
		}
		if (k == 1)
		{	
			printf("minishell: syntax error near unexpected token `%c'\n", c);
			return(-1);
		}
		if (k == 2 || j == 0)
		{
			printf("minishell: error multiline command\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

char	**realoc_argv(char **src)
{
	char **new;
	int len;
	int i;

	i = 0;
	len = argv_len(src);
	new = (char**)malloc(sizeof(char*) * (len + 2));
	char *s;
	while (i < len)
	{
		s = src[i];
		new[i] = src[i];
		i++;
	}
	new[len + 1] = NULL;
	free(src);
	return(new);
}

char	**arg_join(t_all *all, char **argv, char *str)
{
	char **tmp;
	int len;
	char *s;

	if (argv == NULL)
	{
		argv = (char**)malloc(sizeof (char*) * 2);
		if (!argv)
			return (NULL);
		argv[1] = NULL;
		argv[0] = str;
		tmp = argv;
	}
	/* len = 10 {q, q, q, q, q, q, q, q, q, q, NULL}
											9  10 11
		12		{q, q, q, q, q, q, q, q, q, q, n, NULL}							*/
	else
	{
		len = argv_len(argv);
		tmp = realoc_argv(argv);
		if (!tmp)
		{
			print_merror(all);
			return (NULL);
		}
		unshield(str);
		tmp[len] = str;
	}
	return (tmp);
}

char**	tokenize(char *str, t_all *all)
{
	int i;
	char **res;
	char *tmp;
	char *tmp_redir;
	char *str_tmp;

	i = 0;
	res = NULL;
	str_tmp = ft_strtrim(str, " ");
	tmp = ft_strdup("");
	while(str_tmp[i])
	{
		tmp_redir = NULL;
		if (!tmp)
		{
			print_merror(all);
			return (NULL);
		}
		if (str_tmp[i] == ' ')
		{
			if (i > 0 && str_tmp[i - 1] != ' ' && str_tmp[i - 1] != '<' && str_tmp[i - 1] != '>')
			{
				res = arg_join(all, res, tmp);
				if (!tmp)
				{
					print_merror(all);
					return (NULL);
				}
				tmp = ft_strdup("");
				while (str_tmp[i] && str_tmp[i] == ' ')
					i++;
				continue ;
			}
		}
		if (str_tmp[i] != ' ' && str_tmp[i] != '<' && str_tmp[i] != '>')
		{
			tmp = ft_charjoin(tmp, str_tmp[i]);
			if (!tmp)
			{
				print_merror(all);
				return (NULL);
			}
		}
		if (str_tmp[i + 1] == '\0' && str_tmp[i] != '<' && str_tmp[i] != '>' && str_tmp[i] != ' ')
		{
			res = arg_join(all, res, tmp);
			if (!tmp)
			{
				print_merror(all);
				return (NULL);
			}
		}
		if (str_tmp[i] == '<')
		{
			if (str_tmp[i + 1] == '<')
			{
				tmp_redir = ft_strdup("<<");
				if (!tmp_redir)
				{
					print_merror(all);
					return (NULL);
				}
				i++;
			}
			else
			{
				tmp_redir = ft_strdup("<");
				if (!tmp_redir)
				{
					print_merror(all);
					return (NULL);
				}
			}
			if (!ft_strncmp(tmp, "", 2))
				res = arg_join(all, res, tmp_redir);
			else
			{
				res = arg_join(all, res, tmp);
				res = arg_join(all, res, tmp_redir);
				tmp = ft_strdup("");
			}
		}
		if (str_tmp[i] == '>')
		{
			if (str_tmp[i + 1] == '>')
			{
				tmp_redir = ft_strdup(">>");
				if (!tmp_redir)
				{
					print_merror(all);
					return (NULL);
				}
				i++;
			}
			else
			{
				tmp_redir = ft_strdup(">");
				if (!tmp_redir)
				{
					print_merror(all);
					return (NULL);
				}
			}
			if (!ft_strncmp(tmp, "", 2))
				res = arg_join(all, res, tmp_redir);
			else
			{
				res = arg_join(all, res, tmp);
				res = arg_join(all, res, tmp_redir);
				tmp = ft_strdup("");
			}
		}
		i++;
	}
	free(str_tmp);
	//free(tmp);
	free(str);
	//free(tmp_redir);
	return (res);
}

int		m_struct(t_all *all, char ***argv) //ошибка
{
	int i;

	all->cmds = (t_cmd*)malloc(sizeof(t_cmd) * all->cmds_num);
	if (!all->cmds)
		return(print_merror(all));
	i = 0;
	while(i < all->cmds_num)
	{
		all->cmds[i].argv = argv[i];
		i++;
	}
	return (0);
}

void	print_command(t_all *all)
{
	int i;
	int j;
	char *s;

	i = 0;
	while (i < all->cmds_num)
	{
		j = 0;
		while (all->cmds[i].argv[j])
		{
			s = all->cmds[i].argv[j];
			printf("%s\n", s);
			j++;
		}
		i++;
	}
}

void	free_cmd(t_all *all)
{
	int i;
	int j;

	i = 0;
	if (all->cmds)
	{
		while (i < all->cmds_num)
		{
			j = 0;
			while(all->cmds[i].argv[j])
			{
				free(all->cmds[i].argv[j]);
				j++;
			}
			free(all->cmds[i].argv);
			i++;
		}
		free(all->cmds);
		all->cmds = NULL;
	}
}

int		make_struct(t_all *all, char *str)
{
	char **argv;
	int i;
	char **new_argv;
	char ***cmds;

	i = 0;
	argv = ft_split(str, '|');
	if (!argv)
		return (print_merror(all));
	all->cmds_num = argv_len(argv);
	if (check_argv(all, argv) < 0)
		return (-1);
	cmds = (char***)malloc(sizeof(char**) * (all->cmds_num + 1));
	if (!cmds)
		return(print_merror(all));
	cmds[all->cmds_num] = NULL;
	while (argv[i])
	{
		new_argv = tokenize(argv[i], all);
		if (!new_argv)
			return (-1);
		cmds[i] = new_argv;
		i++;
	}
	//execute
	i = 0;
	if (m_struct(all, cmds) < 0)
		return (print_merror(all)); //ЪУЪ!



	///
	execute_cmd(all);
	// free(argv);
	// free (cmds);
	// free_cmd(all);
	return (0);
}

char	*var_replace(t_all *all, char *str, char *d_pointer, char *end)
{
	char *tmp1;
	char *tmp2;

	if ((tmp2 = get_env_val(all->envp, end)))
	{
		tmp1 = ft_substr(str, 0, d_pointer - str);
		tmp1 = my_strjoin(tmp1, tmp2);
		tmp1 = my_strjoin(tmp1, after_var(d_pointer + 1));
		if (!tmp1 || !end)
		{
			print_merror(all);
			return (NULL);
		}
		free(str);
		str = tmp1;
	}
	return (str);
}

char	*if_no_var(t_all *all, char *str, char *d_pointer, char *end)
{
	char *tmp1;
	char *tmp2;

	if (!(tmp2 = get_env_val(all->envp, end)))
	{
		tmp1 = ft_substr(str, 0, d_pointer - str);
		if (!tmp1)
		{
			print_merror(all);
			return (NULL);
		}
		if (ft_isalnum((int)*(d_pointer + 1)) || (char)*(d_pointer + 1) == '_')
			tmp1 = my_strjoin(tmp1, after_var(d_pointer + 1));
		else
		{
			*d_pointer *= -1; // ТАК НАДО
			tmp1 = my_strjoin(tmp1, d_pointer);
		}
		if (!tmp1)
		{
			print_merror(all);
			return (NULL);
		}
		free(str);
		str = tmp1;
	}
	return (str);
}

int		make_cmd(t_all *all)
{
	char **cmds;
	int i;
	char *d_pointer;
	char *tmp1;
	char *tmp2;
	char *end;

	i = 0;
	d_pointer = NULL;
	cmds = ft_split(all->src->str, ';');
	while(cmds[i])
	{
		while ((d_pointer = ft_strchr(cmds[i], '$')))
		{
			end = end_var(d_pointer + 1);
			if (!end)
				return(print_merror(all));
			cmds[i] = var_replace(all, cmds[i], d_pointer, end);
			cmds[i] = if_no_var(all, cmds[i], d_pointer, end);
			free(end);
		}
		make_struct(all, cmds[i]);
		free(cmds[i]);
		i++;
	}
	free(cmds);
	return (0);
}

void	free_all(t_all *all)
{
	if (all->cmds)
		free_cmd(all);
	if (all->src->str)
		free(all->src->str);
}

int		parser(t_all *all)
{
	t_line src;
	int		ret;
	
	ft_bzero(&all->val, sizeof(all->val));
	src.str = ft_strdup(all->history[all->hist_len - 1]);
	src.len = ft_strlen(src.str);
	src.pos = 0;
	ret = shield(all, &src);
	if (ret < 0)
	{
		free(src.str);
		return (0);
	}
	ret = remove_ch(&src);
	if (ret < 0)
	{
		free(src.str);
		print_merror(all);
	}
	all->src = &src;
	make_cmd(all);
	free(src.str);
	//free_all(all);
	return (ret);
}	

void up_arrow(t_all *all)
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
	if (ft_strlen(all->history[all->pos]) > 0)
	{
		tputs(cursor_left, 1, ft_putint);
		all->history[all->pos][ft_strlen(all->history[all->pos]) - 1] = '\0';
		tputs(tgetstr("dc", 0), 1, ft_putint);
	}
}

void 	canon(t_all *all)
{
	all->term.c_lflag |= ECHO;
	all->term.c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, &all->term);
}



int		main(int ac, char **av, char **envp)
{
	t_all *all;
	int len;
	char str[2000];

	all = init_all(envp);
	while(str[0] != '\04') 
	{
		new_line(all, str);
		nocanon(all);
		while (ft_strncmp(str, "\n", 2) && str[0] != '\4')
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
				continue ;
			else if (!ft_strncmp(str, "\e[C", 5))
				continue ;
			else
			{
				if (hist_strjoin(all, str) < 0)
					return (-1);
				write(1, str, len);
			}
		}
		canon(all);
		parser(all);
		save_history(all);
	}
	write(1, "\n", 1);
	return (0);
}


// int main(int ac, char **av, char **envp)
// {
// 	t_all *all;
// 	char str[2000];

// 	all = init_all(envp);
// 	new_line(all, str);
// 	all->pos--;
// 	all->hist_len--;
// 	parser(all);