/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 16:47:45 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/24 17:38:44 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *get_next_line(int fd)
{
    char buffer[1024];
    int i = 0;
    char c;
    
    while (read(fd, &c, 1) > 0 && c != '\n' && i < 1023)
        buffer[i++] = c;
        
    if (i == 0 && c != '\n')
        return NULL;
        
    buffer[i++] = '\n';
    buffer[i] = '\0';
    
    return ft_strdup(buffer);
}

void handle_here_doc(t_pipex *pipex)
{
    char *line;
    int limiter_len;
    
    limiter_len = ft_strlen(pipex->limiter);
    
    // Instrucciones para el usuario
    write(STDOUT_FILENO, "heredoc> ", 9);
    
    // Usar get_next_line o una función similar para leer líneas
    line = get_next_line(STDIN_FILENO);
    
    while (line)
    {
        // Verificar si la línea es el delimitador
        if (ft_strncmp(line, pipex->limiter, limiter_len) == 0 && 
            (line[limiter_len] == '\n' || line[limiter_len] == '\0'))
        {
            free(line);
            break;
        }
        
        // Escribir la línea en el pipe para el primer comando
        write(pipex->here_doc_pipe[1], line, ft_strlen(line));
        free(line);
        
        // Pedir la siguiente línea
        write(STDOUT_FILENO, "heredoc> ", 9);
        line = get_next_line(STDIN_FILENO);
    }
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*part_path;
	char	*full_path;
	int		i;

	i = 0;
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
	free_array(paths);
	return (NULL);
}

void    execute_cmd(t_pipex *pipex, int i)
{
    char *path;

    path = find_path(pipex->procs[i].cmd,pipex->envp);
    if(!path)
    {
        free_pipex(pipex);
        error_exit(ERR_CMD);
    }
    if(execve(path, pipex->procs[i].args, pipex->envp) == -1)
    {
        free(path);
        free_pipex(pipex);
        perror_exit("execve");
    }
    free(path);
}

void error_exit(char *msg)
{
    write(STDERR_FILENO, msg, strlen(msg));
    write(STDERR_FILENO, "\n", 1);
    exit(EXIT_FAILURE);
}

void perror_exit(char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void free_array(char **array)
{
    int i;
    
    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

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

void redirect_io(int in_fd, int out_fd)
{
    dup2(in_fd, STDIN_FILENO);
    dup2(out_fd, STDOUT_FILENO);
}

void close_unused_pipes(t_pipex *pipex, int i)
{
    int j;

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

void launch_pipeline(t_pipex *pipex)
{
    int i;

    create_pipes(pipex);
    
    if (pipex->here_doc)
        handle_here_doc(pipex);
        
    i = 0;
    while (i < pipex->cmd_count)
    {
        if ((pipex->procs[i].pid = fork()) < 0)
            perror_exit(ERR_FORK);
            
        if (pipex->procs[i].pid == 0) // Proceso hijo
        {
            // Configurar la entrada estándar
            if (i == 0)
            {
                if (pipex->here_doc)
                    // Para here_doc, la entrada viene del primer pipe
                    dup2(pipex->procs[0].pipe_fd[0], STDIN_FILENO);
                else
                    // Para modo normal, la entrada viene del archivo de entrada
                    dup2(pipex->fd_in, STDIN_FILENO);
            }
            else
            {
                // Para comandos intermedios y finales, la entrada viene del pipe anterior
                dup2(pipex->procs[i-1].pipe_fd[0], STDIN_FILENO);
            }
            
            // Configurar la salida estándar
            if (i == pipex->cmd_count - 1)
                // Último comando: salida al archivo
                dup2(pipex->fd_out, STDOUT_FILENO);
            else
                // Comandos intermedios: salida al siguiente pipe
                dup2(pipex->procs[i].pipe_fd[1], STDOUT_FILENO);
                
            close_unused_pipes(pipex, i);
            execute_cmd(pipex, i);
        }
        i++;
    }
    
    close_pipes(pipex);
    
    i = 0;
    while (i < pipex->cmd_count)
    {
        waitpid(pipex->procs[i].pid, NULL, 0);
        i++;
    }
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
    
    // Corregir la detección de here_doc
    pipex->here_doc = (ft_strncmp(argv[1], "here_doc", 8) == 0) ? 1 : 0;
    
    if (pipex->here_doc)
    {
        // Modo here_doc
        pipex->limiter = argv[2];
        pipex->cmd_count = argc - 4;
        pipex->fd_in = -1; // Inicializar
        pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND , 0644);
    }
    else
    {
        // Modo normal
        pipex->cmd_count = argc - 3;
        pipex->fd_in = open(argv[1], O_RDONLY);
        pipex->fd_out = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC , 0644);
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