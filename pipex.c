/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:47:45 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/21 13:08:58 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void    free_pipex(t_pipex *pipex)
{
    int i;

    if(pipex->fd_in >= 0)
        close(pipex->fd_in);
    if(pipex->fd_out >= 0)
        close(pipex->fd_out);
    i = 0;
    while(i < pipex->cmd_count)
    {
        if (pipex->procs[i].args)
            free_array(pipex->procs[i].args);
        i++;
    }
    if(pipex->procs)
        free(pipex->procs);
}

void    close_pipes(t_pipex *pipex)
{
    int i;
    i = 0;
    while(i < pipex->cmd_count)
    {
        close(pipex->procs[i].pipe_fd[0]);
        close(pipex->procs[i].pipe_fd[1]);
        i++;
    }
}

void    create_pipes(t_pipex *pipex)
{
    int i;
    i = 0;
    while(i < pipex->cmd_count - 1)
    {
        if(pipe(pipex->procs[i].pipe_fd) == -1)
            perror_exit(ERR_PIPE);
        i++;
    }
}

void    launch_pipeline(t_pipex *pipex)
{
    int i;

    create_pipes(pipex);
    if(pipex->here_doc)
        handle_here_doc(pipex);
    i = 0;
    while(i < pipex->cmd_count)
    {
        if((pipex->procs[i].pid = fork()) < 0)
            perror_exit(ERR_FORK);
        if(pipex->procs[i].pid == 0)
        {
            if(i == 0 && !pipex->here_doc)
                redirect_io(pipex->fd_in,pipex->procs[i].pipe_fd[1]);
            else if(i == pipex->cmd_count - 1)
                dup2(pipex->fd_out,STDOUT_FILENO);
            else
                redirect_io(pipex->procs[i -1 ].pipe_fd[0],pipex->procs[i].pipe_fd[1]);       
        }
        i++;
        execute_cmd(pipex, i);
    }
    close_pipes(pipex);
}

void init_process(t_pipex *pipex)
{
    int i = 0;
    int cmd_index;

    while (i < pipex->cmd_count)
    {
        cmd_index = i + 2 + pipex->here_doc;
        pipex->procs[i].cmd = pipex->argv[cmd_index];
        pipex->procs[i].args = ft_split(pipex->argv[cmd_index], ' ');
        pipex->procs[i].pid = -1;       
        i++;
    }
}

void init_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
    pipex->argc = argc;
    pipex->argv = argv;
    pipex->envp = envp;
    pipex->here_doc = is_here_doc(argv[1]);
    if (pipex->here_doc != 0)
    {
        pipex->limiter = argv[2];
        pipex->cmd_count = argc - 4;
        pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
        pipex->here_doc = 1;
    }
    else
    {
        pipex->cmd_count = argc - 3;
        pipex->fd_in = open(argv[1], O_RDONLY);
        pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    }
    if (pipex->fd_out < 0 || (!pipex->here_doc && pipex->fd_in < 0))
        perror_exit(ERR_FILE);
    pipex->procs = malloc(sizeof(t_process) * pipex->cmd_count);
    if (!pipex->procs)
        perror_exit(ERR_MEM);
    init_process(pipex);
}

int main(int argc, char **argv, char **envp)
{
    t_pipex pipex;
    
    if (argc < 5)
        error_exit(ERR_ARGS);
    
    init_pipex(&pipex, argc, argv, envp);
    launch_pipeline(&pipex);
    free_pipex(&pipex);
    
    return (0);
}