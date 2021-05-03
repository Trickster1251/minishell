/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:15:50 by aemustaf          #+#    #+#             */
/*   Updated: 2020/11/17 15:16:03 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>
#include <string.h>

static char	*check_remainder(char **remainder, char **line)
{
	char *p_new;

	p_new = NULL;
	if (*remainder)
	{
		if ((p_new = my_strchr(*remainder, '\n')))
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

<<<<<<< HEAD
int			get_next_line(int fd, char **line)
=======
static int	ft_read_line(int fd, char **line, char **remainder, t_gnl *g)
{
	g->r_b = read(fd, g->buf, BUFFER_SIZE);
	while (!g->p_new && g->r_b)
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
		g->r_b = read(fd, g->buf, BUFFER_SIZE);
	}
	return (g->r_b);
}

int	get_next_line(int fd, char **line)
>>>>>>> 2c2409a31b7dd95ac73189618b7062f84b73cbc7
{
	t_gnl		g;
	static char	*remainder;

	g.buf = NULL;
	if (fd < 0 || !line || BUFFER_SIZE <= 0 || (read(fd, g.buf, 0) == -1))
		return (-1);
<<<<<<< HEAD
	if (!(buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	p_new = check_remainder(&remainder, line);
	while (!p_new && (readed_byte = read(fd, buf, BUFFER_SIZE)))
	{
		buf[readed_byte] = '\0';
		if ((p_new = my_strchr(buf, '\n')))
		{
			*p_new = '\0';
			if (!(remainder = my_strdup(++p_new)))
				return (errors(buf));
		}
		if (!(*line = my_strjoin(*line, buf)))
			return (errors(buf));
	}
	free(buf);
	return ((p_new || readed_byte) ? 1 : 0);
=======
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
>>>>>>> 2c2409a31b7dd95ac73189618b7062f84b73cbc7
}
