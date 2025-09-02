/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 10:35:54 by rmei              #+#    #+#             */
/*   Updated: 2024/07/01 15:00:52 by rmei             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_makebuffer(int fd, t_buffer *s_buffer, t_line *line)
{
	s_buffer->pos = 0;
	free(s_buffer->buffer);
	s_buffer->buffer = malloc(BUFFER_SIZE);
	if (!s_buffer->buffer)
	{
		s_buffer->end = 0;
		free(line->line);
		line->line = NULL;
		return ;
	}
	s_buffer->end = read(fd, s_buffer->buffer, BUFFER_SIZE);
	if (s_buffer->end <= 0)
	{
		free(s_buffer->buffer);
		s_buffer->buffer = NULL;
		if (s_buffer->end < 0)
		{
			free(line->line);
			line->line = NULL;
			return ;
		}
		if (line->line)
			line->line = ft_realloc(line->line, line->i + 1);
	}
}

static void	ft_makeline(t_buffer *s_buffer, t_line *line)
{
	while (s_buffer->pos < s_buffer->end)
	{
		if (!line->line || line->i >= (int)line->size - 1)
		{
			line->size *= 4;
			line->line = ft_realloc(line->line, line->size);
			if (!line->line)
			{
				free(s_buffer->buffer);
				s_buffer->buffer = NULL;
				break ;
			}
		}
		line->line[line->i++] = s_buffer->buffer[s_buffer->pos];
		line->line[line->i] = '\0';
		if (s_buffer->buffer[s_buffer->pos++] == '\n')
		{
			line->line = ft_realloc(line->line, line->i + 1);
			if (!line->line)
				free(s_buffer->buffer);
			if (!line->line)
				s_buffer->buffer = NULL;
			break ;
		}
	}
}

char	*ft_get_next_line(int fd)
{
	static t_buffer	fd_arr[MAX_PFD];
	t_buffer		*s_buffer;
	t_line			line;

	if (fd < 0 || fd > MAX_PFD)
		return (NULL);
	line.i = 0;
	line.size = LINE_SIZE;
	line.line = NULL;
	s_buffer = &fd_arr[fd];
	while (1)
	{
		if (s_buffer->pos >= s_buffer->end)
			ft_makebuffer(fd, s_buffer, &line);
		if (!s_buffer->buffer)
			return (line.line);
		ft_makeline(s_buffer, &line);
		if (!line.line)
		{
			s_buffer->end = 0;
			return (NULL);
		}
		if (s_buffer->buffer[s_buffer->pos - 1] == '\n')
			return (line.line);
	}
}
