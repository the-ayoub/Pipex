/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:27:23 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/28 16:27:30 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_unused_pipes(t_pipex *pipex, int i)
{
	int	j;

	j = 0;
	while (j < pipex->cmd_count - 1)
	{
		if (j != i - 1)
			close(pipex->procs[j].pipe_fd[0]);
		if (j != i)
			close(pipex->procs[j].pipe_fd[1]);
		j++;
	}
}

void	close_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count)
	{
		if (pipex->procs[i].pipe_fd[0] >= 0)
		{
			close(pipex->procs[i].pipe_fd[0]);
			pipex->procs[i].pipe_fd[0] = -1;
		}
		if (pipex->procs[i].pipe_fd[1] >= 0)
		{
			close(pipex->procs[i].pipe_fd[1]);
			pipex->procs[i].pipe_fd[1] = -1;
		}
		i++;
	}
}

void	create_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		if (pipe(pipex->procs[i].pipe_fd) == -1)
			perror_exit(ERR_PIPE);
		i++;
	}
}
