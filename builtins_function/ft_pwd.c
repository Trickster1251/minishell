#include "../includes/minishell.h"

void	ft_pwd(t_cmd *cmd)
{
	char	dir[1000];
	char	*dir1;

	dir1 = getcwd(dir, 1000);
	if (cmd->argv[1] == NULL)
	{
		write(cmd->fd[1], dir1, strlen(dir1));
		write(cmd->fd[1], "\n", 1);
	}
	else
	{
		printf("pwd: too many arguments\n");
		g_res[0] = 1;
	}
}
