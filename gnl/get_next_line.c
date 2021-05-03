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

char	*return_buf(int fd, char *line, char **b)
{
	char	*buf;

	buf = *b;
	if (fd < 0 || !line || BUFFER_SIZE <= 0 || (read(fd, buf, 0) == -1))
		return (NULL);
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	return (buf);
}

// int	p_new_check(char *p_new, char *remainder, char *buf)
// {
// 	if (p_new)
// 	{
// 		*p_new = '\0';
// 		remainder = my_strdup(++p_new);
// 		if (!(remainder))
// 			return (errors(buf));
// 	}
// 	return (1);
// }

int			get_next_line(int fd, char **line)
{
	char		*buf;
	char		*p_new;
	int			readed_byte;
	static char	*remainder;

	buf = NULL;
	if (fd < 0 || !line || BUFFER_SIZE <= 0 || (read(fd, buf, 0) == -1))
		return (-1);
	buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (buf == NULL)
		return (-1);
	p_new = check_remainder(&remainder, line);
	readed_byte = read(fd, buf, BUFFER_SIZE);
	while (!p_new && readed_byte)
	{
		buf[readed_byte] = '\0';
		p_new = my_strchr(buf, '\n');
		if (p_new)
		{
			*p_new = '\0';
			remainder = my_strdup(++p_new);
			if (!remainder)
				return (errors(buf));
		}
		*line = my_strjoin(*line, buf);
		if (!(*line))
			return (errors(buf));
	}
	free(buf);
	if (p_new || readed_byte)
		return (1);
	return (0);
}