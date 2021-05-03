#include "../includes/minishell.h"

void	check_quotes(t_all *all, t_line *s)
{
	if (s->str[s->pos] == '"' && all->v.dqt == 0
		&& all->v.qt == 0 && prev_c(s) != '\\')
		all->v.dqt = 1;
	else if (s->str[s->pos] == '\''
		&& all->v.qt == 0 && prev_c(s) != '\\')
		all->v.qt = 1;
	else if (s->str[s->pos] == '"' && all->v.dqt == 1
		&& all->v.qt == 0 && prev_c(s) != '\\')
	{
		all->v.dqt = 0;
	}
	else if (s->str[s->pos] == '\''
		&& all->v.qt == 1 && prev_c(s) != '\\')
		all->v.qt = 0;
}

void	shield_sym1(t_all *a, t_line *s)
{
	if (s->str[s->pos] == '&'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == ' '
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == '('
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == ')'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == '|'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == '"'
		&& (a->v.qt == 1 || prev_c(s) == '\\'))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == '\\'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	if (s->str[s->pos] == '\''
		&& (prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
}

void	shield_sym(t_all *a, t_line *s)
{
	if (s->str[s->pos] == '$' && (a->v.qt == 1 || prev_c(s) == '\\')
		&& a->v.dqt != 1)
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == '#'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == ' '
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == '>'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == ';'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	else if (s->str[s->pos] == '<'
		&& (a->v.qt == 1 || prev_c(s) == '\\' || a->v.dqt == 1))
		s->str[s->pos] *= -1;
	shield_sym1(a, s);
}

int	is_right_amp(t_line *s)
{
	if (s->str[s->pos] == '&')
	{
		printf("minishell: syntax error unexpected token `&'\n");
		return (-1);
	}
	return (0);
}

int 	is_right_syntax_pipes(t_line *s, t_all *all, int *k)
{
	if ((s->str[s->pos] == '|' || s->str[s->pos] == '>'
			|| s->str[s->pos] == '<') && s->pos == s->len - 2)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (s->str[s->pos] == '|' && s->pos == 0)
	{
		printf("minishell: syntax error near unexpected token `|'\n");
		return (-1);
	}
	if (is_right_amp(s) < 0)
		return (-1);
	if (s->str[s->pos] == '|')
	{
		*k += 1;
		if (*k > 1)
		{
			printf("minishell: syntax error near unexpected token `|'\n");
			return (-1);
		}
	}
	else
		*k = 0;
	return (0);
}
