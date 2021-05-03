#include "../includes/minishell.h"

void	ft_exit(t_cmd *cmd)
{
	printf("exit\n");
	if (cmd->argv[1] != NULL)
	{
		if (!ft_isdigit_str(cmd->argv[1]))
		{
			printf("minishell: exit: %s: ", cmd->argv[1]);
			printf("numeric arguments required\n");
			g_res[0] = 255;
		}
		else if (cmd->argv[2] != NULL)
		{
			printf("minishell: exit: too many arguments\n");
			g_res[0] = 1;
		}
		else
			g_res[0] = ft_atoi(cmd->argv[1]);
	}
	exit(g_res[0]);
}
