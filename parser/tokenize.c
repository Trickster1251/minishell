#include "../includes/minishell.h"

int		if_space(t_tok *tok, t_all *all, int *i)
{
	tok->res = arg_join(all, tok->res, tok->tmp);
	if (!tok->tmp)
		return (print_merror(all));
	tok->tmp = ft_strdup("");
	while (tok->str_tmp[*i] && tok->str_tmp[*i] == ' ')
		*i += 1;
	return (0);
}

int if_redir(t_all *all, t_tok *tok, int *i)
{
	if (tok->str_tmp[*i] == '<')
	{
		if (tok->str_tmp[*i + 1] == '<')
		{
			tok->tmp_redir = ft_strdup("<<");
			if (!tok->tmp_redir)
				return (print_merror(all));
			*i += 1;
		}
		else
		{
			tok->tmp_redir = ft_strdup("<");
			if (!tok->tmp_redir)
				return (print_merror(all));
		}
		if (!ft_strncmp(tok->tmp, "", 2))
			tok->res = arg_join(all, tok->res, tok->tmp_redir);
		else
		{
			tok->res = arg_join(all, tok->res, tok->tmp);
			tok->res = arg_join(all, tok->res, tok->tmp_redir);
			tok->tmp = ft_strdup("");
		}
	}
	return (0);
}

int if_char(t_all *all, t_tok *tok, int *i)
{
	if (tok->str_tmp[*i] != ' ' && tok->str_tmp[*i] != '<'
	&& tok->str_tmp[*i] != '>')
	{
		tok->tmp = ft_charjoin(tok->tmp, tok->str_tmp[*i]);
		if (!tok->tmp)
			return (print_merror(all));
	}
	if (tok->str_tmp[*i + 1] == '\0' && tok->str_tmp[*i] != '<' 
	&& tok->str_tmp[*i] != '>' && tok->str_tmp[*i] != ' ')
	{
		tok->res = arg_join(all, tok->res, tok->tmp);
		if (!tok->tmp)
			return (print_merror(all));
	}
	return (0);
}

int		if_rev_redir(t_all *all, t_tok *tok, int *i)
{
	if (tok->str_tmp[*i] == '>')
	{
		if (tok->str_tmp[*i + 1] == '>')
		{
			tok->tmp_redir = ft_strdup(">>");
			if (!tok->tmp_redir)
				return (print_merror(all));
			*i += 1;
		}
		else
		{
			tok->tmp_redir = ft_strdup(">");
			if (!tok->tmp_redir)
				return (print_merror(all));
		}
		if (!ft_strncmp(tok->tmp, "", 2))
			tok->res = arg_join(all, tok->res, tok->tmp_redir);
		else
		{
			tok->res = arg_join(all, tok->res, tok->tmp);
			tok->res = arg_join(all, tok->res, tok->tmp_redir);
			tok->tmp = ft_strdup("");
		}
	}
	return (0);
}

char**	tokenize(char *str, t_all *all)
{
	int i;
	t_tok tok;

	i = 0;
	tok.res = NULL;
	tok.str_tmp = ft_strtrim(str, " ");
	tok.tmp = ft_strdup("");
	while(tok.str_tmp[i])
	{
		tok.tmp_redir = NULL;
		if (!tok.tmp)
		{
			print_merror(all);
			return (NULL);
		}
		if (tok.str_tmp[i] == ' ')
		{
			if (i > 0 && tok.str_tmp[i - 1] != ' ' && tok.str_tmp[i - 1] != '<'
			&& tok.str_tmp[i - 1] != '>')
			{
				if (if_space(&tok, all, &i) < 0)
					return (NULL);
				continue ;
			}
		}
		if (if_char(all, &tok, &i) < 0 || if_redir(all, &tok, &i) < 0
		|| if_rev_redir(all, &tok, &i) < 0)
			return (NULL);
		i++;
	}
	free(tok.str_tmp);
	free(str);;
	i = 0;
	return (tok.res);
}
