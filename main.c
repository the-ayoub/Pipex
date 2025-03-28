/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:25:32 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/28 16:25:45 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int		exit_status;
	t_pipex	pipex;

	if (argc < 5)
		error_exit(ERR_ARGS);
	init_pipex(&pipex, argc, argv, envp);
	launch_pipeline(&pipex);
	exit_status = ft_waitpid(&pipex);
	free_pipex(&pipex);
	return (exit_status);
}
