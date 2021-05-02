#include "../includes/minishell.h"

int	is_right_redir_syntax(t_all *all, t_line *src, int *redir, int *rev_redir)
{
	if (src->str[src->pos] == '>')
	{
		*redir += 1;
		if (*redir > 2 || *rev_redir > 0)
		{
			printf("minishell: syntax error near unexpected token `>'\n");
			return (-1);
		}
		if (src->str[src->pos + 1] == '>' && prev_c(src) == '<')
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
	else if (src->str[src->pos] != ' ')
		*redir = 0;
	return (0);
}

int	is_right_revd_syntax(t_all *all, t_line *src, int *rev_redir, int *redir)
{
	if (src->str[src->pos] == '<')
	{
		*rev_redir += 1;
		if (*rev_redir > 2 || *redir > 0)
		{
			printf("minishell: syntax error near unexpected token `<'\n");
			return (-1);
		}
		if (src->str[src->pos + 1] == '>' && prev_c(src) == '<')
		{
			printf("minishell: syntax error near unexpected token `<'\n");
			return (-1);
		}
	}
	else if (src->str[src->pos] != ' ')
		*rev_redir = 0;
	return (0);
}

char	*end_var(char *s)
{
	char	*s1;
	char	*tmp;

	s1 = s;
	while (*s1 && (ft_isalnum(*s1) || *s1 == '_'))
		s1++;
	tmp = ft_substr(s, 0, s1 - s);
	return (tmp);
}

char	*after_var(char *s)
{
	while (*s && (ft_isalnum(*s) || *s == '_'))
		s++;
	return (s);
}

int	print_syntax_err(char c)
{
	printf("minishell: syntax error near unexpected token `%c'\n", c);
	return (-1);
}
