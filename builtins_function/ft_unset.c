#include "../includes/minishell.h"

void    ft_unset(t_cmd *cmd, t_list *envp, t_list *exp)
{
	int     len;
	int     i;

	len = len_arr(cmd->argv);
	i = 0;
	if (cmd->argv[1] != NULL)
	{
		while(++i < len && cmd->argv[i])
			del_key(&exp, cmd->argv[i]);
			// del_key(&envp, cmd->argv[i]);
	}
}
