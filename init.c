/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:25:00 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/28 17:03:20 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_process(t_pipex *pipex)
{
	int	i;
	int	cmd_index;

	i = 0;
	if (access(pipex->argv[1], F_OK) != 0 && !pipex->here_doc)
		perror(ERR_FILE);
	else if (!pipex->here_doc && pipex->fd_in < 0)
		perror_exit(ERR_FILE);
	pipex->procs = malloc(sizeof(t_process) * pipex->cmd_count);
	if (!pipex->procs)
		perror_exit(ERR_MEM);
	while (i < pipex->cmd_count)
	{
		cmd_index = i + 2 + pipex->here_doc;
		pipex->procs[i].cmd = pipex->argv[cmd_index];
		pipex->procs[i].args = ft_split(pipex->argv[cmd_index], ' ');
		pipex->procs[i].pid = -1;
		pipex->procs[i].pipe_fd[0] = -1;
		pipex->procs[i].pipe_fd[1] = -1;
		i++;
	}
}

void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
	pipex->argc = argc;
	pipex->argv = argv;
	pipex->envp = envp;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		pipex->here_doc = 1;
	else
		pipex->here_doc = 0;
	if (pipex->here_doc)
	{
		pipex->limiter = argv[2];
		pipex->cmd_count = argc - 4;
		pipex->fd_in = -1;
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND,
				0644);
	}
	else
	{
		pipex->cmd_count = argc - 3;
		pipex->fd_in = open(argv[1], O_RDONLY);
		pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC,
				0644);
	}
	init_process(pipex);
}
