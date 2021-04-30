#include "includes/minishell.h"

void    what_is_redir(int i, t_cmd *cmd, char **arr)
{
    if (ft_strncmp(arr[i - 1], "<\0", 2) == 0)
    {
        if (cmd->fd[0] != 0)
            close(cmd->fd[0]);
        cmd->fd[0] = open(arr[i], O_RDONLY);
    }
    else if (ft_strncmp(arr[i - 1], ">\0", 2) == 0)
    {
        if (cmd->fd[1] != 1)
            close(cmd->fd[1]);
        cmd->fd[1] = open(arr[i], O_TRUNC | O_RDWR | O_CREAT, 0666);
    }
    else if (ft_strncmp(arr[i - 1], ">>\0", 3) == 0)
    {
        if (cmd->fd[1] != 1)
            close(cmd->fd[1]);
        cmd->fd[1] = open(arr[i], O_APPEND | O_RDWR | O_CREAT, 0666);
    }
}

void    create_open_fd(t_all *a, t_cmd *cmd, char **arr)
{
    int     i;
    t_list  *lst = NULL;
    i = 0;
    if (!is_redir_type(arr[i]))
        ft_lstadd_back(&lst, ft_lstnew(ft_strdup(arr[i])));
    while(arr[++i])
    {
        if (!is_redir_type(arr[i]) && !is_redir_type(arr[i - 1]))
            ft_lstadd_back(&lst, ft_lstnew(ft_strdup(arr[i])));
        else
            what_is_redir(i, cmd, arr);
        if (errno != 0)
        {
            printf("%s\n", strerror(errno));
            errno = 0;
            gl_fd[0] = 1;
            break ;
        }
    }
	i = 0;
	while (cmd->argv[i])
	{
		free(cmd->argv[i]);
		i++;
	}
	free(cmd->argv);
    cmd->argv = lst_to_argv(lst);
	ft_lstclear(&lst, free);
}

int     is_redir_type(char *str)
{
    return (ft_strncmp(str, ">\0", 2) == 0
    || ft_strncmp(str, ">>\0", 3) == 0
    || ft_strncmp(str, "<", 2) == 0);
}

int     count_redir(char **arr)
{
    int     i;
    int     count;

    i = -1;
    count = 0;
    while(arr[++i])
    {
        if (ft_strncmp(arr[i], ">\0", 2) == 0 ||
        ft_strncmp(arr[i], ">>\0", 3) == 0 ||
        ft_strncmp(arr[i], "<\0", 2) == 0)
            count++;
    }
    return (count);
}