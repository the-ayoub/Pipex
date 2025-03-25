/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:57:44 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/25 20:50:28 by aybelhaj         ###   ########.fr       */
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
	char		*cmd;
	char		**args;
	int			pipe_fd[2];
	pid_t		pid;
}				t_process;

typedef struct s_pipex
{
	int			argc;
	char		**argv;
	char		**envp;
	int			fd_in;
	int			fd_out;
	int			here_doc;
	char		*limiter;
	int			cmd_count;
	int			here_doc_pipe[2];
	t_process	*procs;
}				t_pipex;

// main.c
int				main(int argc, char **argv, char **envp);

// init.c
void			init_pipex(t_pipex *pipex, int argc, char **argv, char **envp);
void			init_process(t_pipex *pipex);

// execute.c
void			launch_pipeline(t_pipex *pipex);
//void			ft_waitpid(t_pipex *pipex);
void			execute_cmd(t_pipex *pipex, int i);
char			*find_path(char *cmd, char **envp);

// pipes.c
void			create_pipes(t_pipex *pipex);
void			close_pipes(t_pipex *pipex);
void			close_unused_pipes(t_pipex *pipex, int i);

// here_doc.c (bonus)
void			handle_here_doc(t_pipex *pipex);

// utils.c
void			error_exit(char *msg);
void			perror_exit(char *msg);
void			free_pipex(t_pipex *pipex);
void			free_array(char **array);

#endif