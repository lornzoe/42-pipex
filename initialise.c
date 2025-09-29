/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialise.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 05:18:46 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 07:15:24 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	initialise_files(int *infile, int *outfile, char **argv, int argc)
{
	*infile = open(argv[1], O_RDONLY);
	if (*infile < 0)
	{
		perror("Error opening infile");
	}
	*outfile = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (*outfile < 0)
	{
		perror("Error opening/creating outfile");
		return (1);
	}
	return (0);
}

int	initialise_pipe(int pipefd[2], int infile, int outfile)
{
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		close_2(infile, outfile);
		return (-1);
	}
	return (0);
}

int	initialise_fork(int *pid, int files[2], int pipefd[2])
{
	*pid = fork();
	if (*pid < 0)
	{
		perror("Error forking process");
		close_4(files[INFILE], files[OUTFILE], pipefd[0], pipefd[1]);
		exit(1);
	}
	return (0);
}
