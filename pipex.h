/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:57:44 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/24 17:42:12 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "./libft/libft.h"
# include <fcntl.h>
# include <sys/wait.h>

# define ERR_ARGS "Error: Invalid number of arguments"
# define ERR_PIPE "Error: Pipe creation failed"
# define ERR_FORK "Error: Fork failed"
# define ERR_FILE "Error: File operation failed"
# define ERR_CMD "Error: Command not found"
# define ERR_MEM "Error: Memory allocation failed"

typedef struct s_process 
{
    char    *cmd;       // Comando completo
    char    **args;     // Argumentos parseados
    int     pipe_fd[2]; // Pipe para este proceso
    pid_t   pid;        // PID del proceso
} t_process;

typedef struct s_pipex 
{
    int     argc;      
    char    **argv;
    char    **envp;
    int     fd_in;
    int     fd_out;
    int     here_doc;   // Flag para here_doc
    char    *limiter;   // Delimitador para here_doc
    int     cmd_count;  // Número de comandos
    int     here_doc_pipe[2]; // Pipe específico para here_doc
    t_process *procs;   // Array de procesos
} t_pipex;

// main.c
int     main(int argc, char **argv, char **envp);

// init.c
void    init_pipex(t_pipex *pipex, int argc, char **argv, char **envp);
void    init_process(t_pipex *pipex);

// execute.c
void    launch_pipeline(t_pipex *pipex);
void    execute_cmd(t_pipex *pipex, int i);
char    *find_path(char *cmd, char **envp);

// pipes.c
void    create_pipes(t_pipex *pipex);
void    close_pipes(t_pipex *pipex);
void    redirect_io(int in_fd, int out_fd);
void    close_unused_pipes(t_pipex *pipex, int i);

// here_doc.c (bonus)
void    handle_here_doc(t_pipex *pipex);

// utils.c
void    error_exit(char *msg);
void    perror_exit(char *msg);
void    free_pipex(t_pipex *pipex);
void    free_array(char **array);

#endif