/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rromero <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/14 14:34:24 by rromero           #+#    #+#             */
/*   Updated: 2016/11/14 14:34:26 by rromero          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

t_file	*new_file(const int fd)
{
	t_file	*file;

	if (!(file = (t_file *)ft_memalloc(sizeof(t_file) + 1)))
		return (0);
	(*file).fd = fd;
	file->tmp = 0;
	file->l_len = 0;
	return (file);
}

void	read_buf(char *b, t_file *file, long r)
{
	size_t		l_size;
	char		*nl;

	nl = 0;
	if ((nl = ft_strchr(b, '\n')) || (!*b && file->line))
	{
		(*file).ret = 1;
		if ((nl) && (nl + 1))
		{
			file->tmp = ft_strdup(nl + 1);
			*nl = 0;
		}
	}
	else if (!file->line && !*b && !r)
		(*file).ret = 0;
	l_size = (file->line) ? ft_strlen(file->line) : 0;
	(*file).l_len += ft_strlen(b);
	file->line = (char *)ft_realloc(file->line, l_size, ((*file).l_len + 1));
	(void)ft_strcat(file->line, b);
}

int		read_next_line(char **line, t_file *file)
{
	char		*buf;
	long		r;

	buf = ft_strnew(BUFF_SIZE);
	file->line = 0;
	(*file).ret = -1;
	r = 0;
	while ((*file).ret < 0)
	{
		if (file->tmp && *file->tmp)
		{
			if (*file->tmp)
				(void)ft_strcat(buf, file->tmp);
			ft_strdel(&file->tmp);
			file->tmp = 0;
		}
		else if ((r = read((*file).fd, buf, BUFF_SIZE)) < 0)
			return ((*file).ret);
		(void)read_buf(&*buf, file, r);
		(void)ft_bzero(buf, BUFF_SIZE);
	}
	ft_strdel(&buf);
	*line = file->line;
	return ((*file).ret);
}

int		get_next_line(const int fd, char **line)
{
	static t_node	*file_lst;
	t_node			*node;
	t_node			*new;
	t_file			*c_file;

	if (fd < 0 || !line)
		return (-1);
	if (!file_lst)
		file_lst = ft_lstnew((void *)new_file(fd), sizeof(t_file));
	node = (t_node *)file_lst;
	c_file = 0;
	while ((node) && (c_file = node->data))
	{
		if (c_file->fd == fd)
			return (read_next_line(line, c_file));
		node = node->next;
	}
	new = (t_node *)ft_memalloc(sizeof(t_node));
	new->data = (t_file *)new_file(fd);
	(void)ft_lstadd(&file_lst, new);
	return (read_next_line(line, (t_file *)new->data));
}
