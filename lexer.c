#include "includes/minishell.h"

void	check_quotes(t_all *all, t_line *src)
{
	if (src->str[src->pos] == '"' && all->val.in_dqt == 0 && all->val.in_qt == 0 && prev_char(src) != '\\')
		all->val.in_dqt = 1;
	else if (src->str[src->pos] == '\'' && all->val.in_qt == 0 && prev_char(src) != '\\')
		all->val.in_qt = 1;
	else if (src->str[src->pos] == '"' && all->val.in_dqt == 1 && all->val.in_qt == 0 && prev_char(src) != '\\')
	{
		all->val.in_dqt = 0;
	}
	else if (src->str[src->pos] == '\'' && all->val.in_qt == 1 && prev_char(src) != '\\')
		all->val.in_qt = 0;
}

void shield_sym(t_all *all, t_line *src)
{
	if (src->str[src->pos] == '$' && (all->val.in_qt == 1 || prev_char(src) == '\\'))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '#' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == ' ' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '>' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == ';' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '<' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '&' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == ' ' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '(' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == ')' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '|' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '"' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	else if (src->str[src->pos] == '\\' && (all->val.in_qt == 1 || prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
	if (src->str[src->pos] == '\'' && (prev_char(src) == '\\' || all->val.in_dqt == 1))
		src->str[src->pos] *= -1;
}

int 	is_right_syntax_pipes(t_line *src, t_all *all, int *k)
{
	if ((src->str[src->pos] == '|' || src->str[src->pos] == '>'
	|| src->str[src->pos] == '<') && src->pos == src->len - 2)
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

int		is_right_redir_syntax(t_all *all, t_line *src, int *redir, int *rev_redir)
{
	if (src->str[src->pos] == '>')
	{
		*redir += 1;
		if (*redir > 2 || *rev_redir > 0)
		{
			printf("minishell: syntax error near unexpected token `>'\n");
			return (-1);
		}
		if (src->str[src->pos + 1] == '>' && prev_char(src) == '<')
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

int		is_right_revd_syntax(t_all *all, t_line *src, int *rev_redir, int *redir)
{
	if (src->str[src->pos] == '<')
	{
		*rev_redir += 1;
		if (*rev_redir > 2 || *redir > 0)
		{
			printf("minishell: syntax error near unexpected token `<'\n");
			return (-1);
		}

		if (src->str[src->pos + 1] == '>' && prev_char(src) == '<')
		{
			printf("minishell: syntax error near unexpected token `<'\n");
			return (-1);
		}
	}
	else if (src->str[src->pos] != ' ')
		*rev_redir = 0;
	return (0);
}