#include "includes/minishell.h"

int		parser(t_all *all)
{
	t_line src;
	int		ret;
	
	if (all->hist == NULL || all->hist_len <= 0)
		return (0);
	ft_bzero(&all->val, sizeof(all->val));
	src.str = ft_strdup(all->hist[all->hist_len - 1]);
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
	return (ret);
}	

int		make_cmd(t_all *all)
{
	char **cmds;
	int i;
	char *d_pointer;
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