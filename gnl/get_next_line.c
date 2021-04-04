/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aemustaf <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:15:50 by aemustaf          #+#    #+#             */
/*   Updated: 2021/01/31 22:41:18 by aemustaf         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


static char	*check_remainder(char **remainder, char **line)
{
	char *p_new;

	p_new = NULL;
	if (*remainder)
	{
		if ((p_new = ft_strchr(*remainder, '\n')))
		{
			*p_new = '\0';
			p_new++;
			*line = *remainder;
			*remainder = ft_strdup(p_new);
		}
		else
		{
			*line = *remainder;
			*remainder = NULL;
		}
	}
	else
		*line = ft_strdup("");
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

int			get_next_line(int fd, char **line)
{
	char		*buf;
	char		*p_new;
	int			readed_byte;
	static char	*remainder;

	buf = NULL;
	if (fd < 0 || !line || BUFFER_SIZE <= 0 || (read(fd, buf, 0) == -1))
		return (-1);
	if (!(buf = (char*)malloc(sizeof(char) * (BUFFER_SIZE + 1))))
		return (-1);
	p_new = check_remainder(&remainder, line);
	while (!p_new && (readed_byte = read(fd, buf, BUFFER_SIZE)))
	{
		buf[readed_byte] = '\0';
		if ((p_new = ft_strchr(buf, '\n')))
		{
			*p_new = '\0';
			if (!(remainder = ft_strdup(++p_new)))
				return (errors(buf));
		}
		if (!(*line = my_strjoin(*line, buf)))
			return (errors(buf));
	}
	free(buf);
	return ((p_new || readed_byte) ? 1 : 0);
}

