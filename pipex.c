/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:58:06 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/14 16:06:32 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

process_child1(char **argv,char **envp)
{
	t_pipex pp = NULL;

	pp->infile = open(argv[1], O_RDONLY);
}
process_child2()
{
	t_pipex pp = NULL;

	pp->outfile = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0777);
}
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pp = NULL;

	if (argc >= 5)
	{
		if(pipe(pp->fd) = -1)
			error();
		if ((pp->pid1 = fork()) == 0)
			process_child1();
		if ((pp->pid1 = fork()) == -1)
			error();
		if ((pp->pid2 = fork()) == 0)
			process_child2();
		if ((pp->pid2 = fork()) == -1)
			error();
		waitpid(pp->pid1);
		waitpid(pp->pid2);
	}
	else
		error();
}
