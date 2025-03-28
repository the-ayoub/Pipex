/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:28:08 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/28 16:30:03 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_exit(char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	exit(EXIT_FAILURE);
}

void	perror_exit(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_pipex(t_pipex *pipex)
{
	int	i;

	if (pipex->fd_in >= 0)
		close(pipex->fd_in);
	if (pipex->fd_out >= 0)
		close(pipex->fd_out);
	i = 0;
	while (i < pipex->cmd_count)
	{
		if (pipex->procs[i].args)
			free_array(pipex->procs[i].args);
		i++;
	}
	if (pipex->procs)
		free(pipex->procs);
}
