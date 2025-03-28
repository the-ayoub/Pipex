/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:24:40 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/28 16:24:46 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_next_line(int fd)
{
	char	buffer[1024];
	int		i;
	char	c;

	i = 0;
	while (read(fd, &c, 1) > 0 && c != '\n' && i < 1023)
		buffer[i++] = c;
	if (i == 0 && c != '\n')
		return (NULL);
	buffer[i++] = '\n';
	buffer[i] = '\0';
	return (ft_strdup(buffer));
}

void	handle_here_doc(t_pipex *pipex)
{
	char	*line;
	int		limiter_len;

	if (pipe(pipex->here_doc_pipe) == -1)
		perror_exit("Pipe creation failed");
	limiter_len = ft_strlen(pipex->limiter);
	while (1)
	{
		write(STDOUT_FILENO, "heredoc> ", 9);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, pipex->limiter, limiter_len) == 0
			&& (line[limiter_len] == '\n' || line[limiter_len] == '\0'))
		{
			free(line);
			break ;
		}
		write(pipex->here_doc_pipe[1], line, ft_strlen(line));
		free(line);
	}
	close(pipex->here_doc_pipe[1]);
}
