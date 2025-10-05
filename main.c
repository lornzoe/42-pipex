/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:19:41 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/05 16:29:07 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	check_argc(int argc)
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile\n",
			STDERR_FILENO);
		exit(1);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		files[3];
	int		pipefd[2];
	int		status;

	check_argc(argc);
	files[FILEERROR_CHECK] = initialise_files(&files[INFILE],
			&files[OUTFILE], argv, argc);
	if (initialise_pipe(pipefd, files[INFILE], files[OUTFILE]) != 0)
		return (1);
	status = run_pids(files, pipefd, argv, envp);
	if (files[FILEERROR_CHECK])
		return (1);
	if (status != 0)
		return (status);
	return (0);
}
