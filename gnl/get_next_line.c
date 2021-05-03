#include "get_next_line.h"
#include <stdio.h>
#include <string.h>

static char	*check_remainder(char **remainder, char **line)
{
	char	*p_new;

	p_new = NULL;
	if (*remainder)
	{
		p_new = my_strchr(*remainder, '\n');
		if (p_new)
		{
			*p_new = '\0';
			p_new++;
			*line = *remainder;
			*remainder = my_strdup(p_new);
		}
		else
		{
			*line = *remainder;
			*remainder = NULL;
		}
	}
	else
		*line = my_strdup("");
	return (p_new);
}

static int	errors(char *buf)
{
	if (buf)
	{
		free(buf);
		buf = NULL;
	}
	return (-1);
}

static int	my_read(int fd, t_gnl *g)
{
	g->r_b = read(fd, g->buf, BUFFER_SIZE);
	return (g->r_b);
}

static int	ft_read_line(int fd, char **line, char **remainder, t_gnl *g)
{
	while (!g->p_new && my_read(fd, g))
	{
		g->buf[g->r_b] = '\0';
		g->p_new = my_strchr(g->buf, '\n');
		if (g->p_new)
		{
			*g->p_new = '\0';
			*remainder = my_strdup(++g->p_new);
			if (!*(remainder))
				return (errors(g->buf));
		}
		*line = my_strjoin(*line, g->buf);
		if (!(*line))
			return (errors(g->buf));
	}
	return (g->r_b);
}

int	get_next_line(int fd, char **line)
{
	t_gnl		g;
	static char	*remainder;

	g.buf = NULL;
	if (fd < 0 || !line || BUFFER_SIZE <= 0 || (read(fd, g.buf, 0) == -1))
		return (-1);
	g.buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!g.buf)
		return (-1);
	g.p_new = check_remainder(&remainder, line);
	g.r_b = ft_read_line(fd, line, &remainder, &g);
	if (g.r_b < 0)
		return (-1);
	free(g.buf);
	if (g.p_new || g.r_b)
		return (1);
	else
		return (0);
}
