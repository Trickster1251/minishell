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

// int words_len(char **words)
// {
// 	int i;

// 	i = 0;
// 	if (**words == NULL)
// 		return (0);
// 	while (words[i])
// 		i++;
// 	return (i);
// }

// char **wordsdup(char** words)
// {
// 	char **copy;

// 	int i;

// 	i = words_len(words);
// 	copy = (char**)ft_calloc(sizeof(char *), i);

// 	while(*words)
// 	{
		
// 	}
// }

t_all *init_all(char **env)
{
	t_all *all;

	all = (t_all*)malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	//all->env_strs = ft_strdup(env);
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

int		main(int ac, char **av, char **env)
{
	t_all *all;
	int len;
	char str[2000];
	struct termios term;

	all = init_all(env);
	char *term_name = get_env_val(all->env, "TERM");
	ft_bzero(str, 2000);
	tcgetattr(0, &term);
	term.c_lflag &= ~(ECHO);
	term.c_lflag &= ~(ICANON);
	tcsetattr(0, TCSANOW, &term);
	tgetent(0, term_name);
	tputs("minishell$ ", 1, ft_putint);
	tputs(save_cursor, 1, ft_putint);
	while(ft_strncmp(str, "\n", 2))
	{
		len = read(0, str, 100);
		str[len] = 0;
		if (!ft_strncmp(str, "\e[A", 5))
		{
			tputs(restore_cursor, 1, ft_putint);
			write(1, "previus", 8);
			tputs(tigetstr("ed"), 1, ft_putint);
		}
		else if (!ft_strncmp(str, "\e[B",  5))
		{
			tputs(restore_cursor, 1, ft_putint);
			write(1, "next", 4);
			tputs(tigetstr("ed"), 1, ft_putint);
		}
		else if (!ft_strncmp(str, "\177", 5))
		{
			tputs(cursor_left, 1, ft_putint);
			tputs(tgetstr("dc", 0), 1, ft_putint);
		}
		else if (!ft_strncmp(str, "\e[D", 5))
		{
			tputs(cursor_left, 1, ft_putint);
		}
		else if (!ft_strncmp(str, "\e[C", 5))
		{
			tputs(cursor_right, 1, ft_putint);
		}
		else
			write(1, str, len);
	}
	term.c_lflag |= ECHO;
	term.c_lflag |= ICANON;
	tcsetattr(0, TCSANOW, &term);
	write(1, "\n", 1);
	return (0);
}