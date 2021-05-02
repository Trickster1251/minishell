#include "../includes/minishell.h"

char	prev_char(t_line *src)
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
		check_quotes(all, src);
		shield_sym(all, src);
		if (is_right_syntax_pipes(src, all, &k) < 0)
			return (-1);
		if (is_right_redir_syntax(all, src, &redir, &rev_redir) < 0)
			return (-1);
		if (is_right_revd_syntax(all, src, &rev_redir, &redir) < 0)
			return(-1);
		src->pos++;
	}
	if (all->val.in_dqt == 1 || all->val.in_qt == 1)
	{
		printf("minishell: quote is not closed\n");
		return (-1);
	}
	return (0);
}

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