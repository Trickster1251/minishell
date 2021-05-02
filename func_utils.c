#include "includes/minishell.h"

int     who_more(char *str, char *str1)
{
    if (ft_strlen(str) >= ft_strlen(str1))
        return (ft_strlen(str));
    return (ft_strlen(str1));
}

int     ft_isdigit_str(char *str)
{
    int     i;

    i = -1;
    while(str[++i])
        if (!ft_isdigit(str[i]))
            return (0);
    return(1);
}

int     len_arr(char **str)
{
    int i;

    i = -1;
    while(str[++i]);
    return (i);
}

int     is_min(int n, int m)
{
    if (n >= m)
        return (m);
    return (n);
}

int     is_max(int n, int m)
{
    if (n >= m)
        return (n);
    return (m);
}

int     is_delim(char *line)
{
    int     i;

    i = -1;
    while(line[++i])
    {
        if (line[i] == '=')
            return (i);
    }
    return (0);
}