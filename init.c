#include "includes/minishell.h"

void	shlvl_ini(t_all *all)
{
	char	*shlvl;
	int		val;
	t_env	*env;

	shlvl = NULL;
	shlvl = get_env_val(all->envp, "SHLVL");
	if (shlvl != NULL)
		val = ft_atoi(shlvl);
	else
		return ;
	if (val < 0)
		add_key(all->envp, "SHLVL", "0", 0);
	else if (val >= 1000)
	{
		printf("minishell: warning: shell level (%d) too", val + 1);
		printf(" high, resetting to 1\n");
		add_key(all->envp, "SHLVL", "1", 0);
	}
	else
	{	
		val++;
		add_key(all->envp, "SHLVL", ft_itoa(val), 0);
	}
	if (shlvl)
		free(shlvl);
}

void	new_env(t_all *all)
{
	t_list	*tmp_list;
	t_env	*env;
	char	**new_env;
	int		i;

	tmp_list = all->envp;
	i = 0;
	new_env = calloc(ft_lstsize(tmp_list) + 1, sizeof(char *));
	while (tmp_list)
	{
		env = tmp_list->content;
		new_env[i] = ft_strjoin(env->key, "=");
		new_env[i] = my_strjoin(new_env[i], env->value);
		tmp_list = tmp_list->next;
		i++;
	}
	all->env_strs = new_env;
}

void 	canon(t_all *all)
{
	all->term.c_lflag |= ECHO;
	all->term.c_lflag |= ICANON;
	all->term.c_lflag |= ISIG;
	tcsetattr(0, TCSANOW, &all->term);
}

void	nocanon(t_all *all)
{
	all->term_name = get_env_val(all->envp, "TERM");
	tcgetattr(0, &all->term);
	all->term.c_lflag &= ~(ECHO);
	all->term.c_lflag &= ~(ICANON);
	all->term.c_lflag &= ~(ISIG);
	tcsetattr(0, TCSANOW, &all->term);
	tgetent(0, all->term_name);
	tputs("minishell$ ", 1, ft_putint);
	tputs(save_cursor, 1, ft_putint);
}

t_all	*init_all(char **envp)
{
	t_all	*all;

	all = (t_all *)malloc(sizeof(t_all));
	if (!all)
		return (NULL);
	all->hist_list = NULL;
	all->hist = NULL;
	all->hist_len = 0;
	all->envp = parse_env(envp);
	all->home = getenv("HOME");
	all->file = ft_strjoin(all->home, "/.minishell_history");
	all->cmds = NULL;
	all->src = NULL;
	return (all);
}
