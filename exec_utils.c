#include "includes/minishell.h"

char	*absolute_path(t_cmd *cmd)
{
	int	fd;

	fd = open(cmd->argv[0], O_RDONLY);
	if (fd)
	{
		if (ft_strncmp("./", cmd->argv[0], 2))
		{
			printf("minishell: %s: command not found\n", cmd->argv[0]);
			g_res[0] = 127;
			return (NULL);
		}
	}
	return (cmd->argv[0]);
}

char	*is_binary(int i, t_cmd *cmd, char **path)
{
	char		*tmp;
	char		*comd;
	struct stat	buf;

	comd = ft_strjoin("/", cmd->argv[0]);
	tmp = ft_strjoin(path[i], comd);
	if (lstat(tmp, &buf) == 0)
	{
		free(tmp);
		tmp = ft_strjoin(path[i], comd);
		free_str_arr(path);
		free(comd);
		return (tmp);
	}
	else
		free(tmp);
	free(comd);
	return (NULL);
}

void	free_pfd(t_all *a, pid_t ***p)
{
	pid_t	**pfd;
	int		i;

	i = -1;
	pfd = *p;
	if (pfd != 0)
	{
		while (++i < a->cmds_num - 1)
			free(pfd[i]);
		free(pfd);
	}
}
