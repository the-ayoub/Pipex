/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:24:14 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/28 16:36:34 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	launch_pipeline(t_pipex *pipex)
{
	int	i;

	create_pipes(pipex);
	if (pipex->here_doc)
		handle_here_doc(pipex);
	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->procs[i].pid = fork();
		if (pipex->procs[i].pid < 0)
			perror_exit(ERR_FORK);
		if (pipex->procs[i].pid == 0)
			is_child(pipex, i);
		i++;
	}
}

int	ft_waitpid(t_pipex *pipex)
{
	int	status;
	int	last_exit_status;
	int	i;

	last_exit_status = 0;
	close_pipes(pipex);
	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->procs[i].pid, &status, 0);
		i++;
	}
	waitpid(pipex->procs[pipex->cmd_count - 1].pid, &status, 0);
	if (WIFEXITED(status))
		last_exit_status = WEXITSTATUS(status);
	return (last_exit_status);
}

void	execute_cmd(t_pipex *pipex, int i)
{
	char	*path;

	path = find_path(pipex->procs[i].args[0], pipex->envp, 0);
	if (!path)
	{
		free_pipex(pipex);
		error_exit(ERR_CMD);
	}
	if (execve(path, pipex->procs[i].args, pipex->envp) == -1)
	{
		free(path);
		free_pipex(pipex);
		perror_exit("execve");
	}
	free(path);
}

char	*find_path(char *cmd, char **envp, int i)
{
	char	**paths;
	char	*part_path;
	char	*full_path;

	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (!full_path)
			return (free_array(paths), NULL);
		if (access(full_path, X_OK) == 0)
			return (free_array(paths), full_path);
		free(full_path);
		i++;
	}
	return (free_array(paths), NULL);
}

void	is_child(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		if (pipex->here_doc)
		{
			dup2(pipex->here_doc_pipe[0], STDIN_FILENO);
			close(pipex->here_doc_pipe[0]);
		}
		else
			dup2(pipex->fd_in, STDIN_FILENO);
	}
	else
		dup2(pipex->procs[i - 1].pipe_fd[0], STDIN_FILENO);
	if (i == pipex->cmd_count - 1)
		dup2(pipex->fd_out, STDOUT_FILENO);
	else
		dup2(pipex->procs[i].pipe_fd[1], STDOUT_FILENO);
	close_unused_pipes(pipex, i);
	execute_cmd(pipex, i);
}
