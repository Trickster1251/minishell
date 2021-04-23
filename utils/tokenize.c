#include "../includes/minishell.h"

t_command	*create_cmd(char **argv)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_bzero(cmd, sizeof(t_command));
	cmd->args = argv;
	return (cmd);
}

void	put_cmd_fd(t_command *cmd, int in, int out)
{
	cmd->fd[0] = in;
	cmd->fd[1] = out;
}

int		get_branch_size(t_command *cmd)
{
	t_command	*tmp;
	int			size;

	tmp = cmd;
	size = 0;
	while (tmp)
	{
		tmp = tmp->child;
		size++;
	}
	return(size);
}

void	put_cmd_child(t_command *cmd, t_command *child)
{
	t_command	*tmp;
	int count;

	tmp = cmd;
	count = get_branch_size(cmd) + 1;
	if (!cmd)
		cmd = child;
	while (tmp)
	{
		tmp->children_num = count;
		count--;
		tmp->child = child;
		tmp = tmp->child;
	}
}

void put_onichan(t_command *cmd, t_command *onichan)
{
	t_command *tmp;
	
	if (!onichan)
		return ;
	if (!cmd)
		cmd = onichan;
	else
	{
		tmp = cmd;
		while (!tmp->onichan)
			tmp = tmp->onichan;
		tmp->onichan = onichan;
	}
}
