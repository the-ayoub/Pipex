/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybelhaj <aybelhaj@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:57:44 by aybelhaj          #+#    #+#             */
/*   Updated: 2025/03/14 17:38:40 by aybelhaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "./libft/libft.h" 

typedef struct s_pipex
{
	int		fd[2];
	int		infile;
	int		outfile;
	t_pid	pid1;
	t_pid	pid2;
}			t_pipex;

#endif
