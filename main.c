/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 15:16:12 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/05 19:27:04 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void	check_argc(int argc)
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile\n",
			STDERR_FILENO);
		exit(1);
	}
}

static void	init_pipex(t_pipex *pipex)
{
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->status = 0;
	pipex->filestatus = 0;
}

static void	cleanup_pipex_fds(t_pipex pipex)
{
	if (pipex.infile >= 0)
		close(pipex.infile);
	pipex.infile = -1;
	if (pipex.outfile >= 0)
		close(pipex.outfile);
	pipex.outfile = -1;
}

static char	**build_cmds(int argc, char **argv)
{
	char	**cmds;
	int		i;

	cmds = malloc(sizeof(char *) * (argc - 2));
	i = 0;
	while (i < argc - 3)
	{
		cmds[i] = argv[i + 2];
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	char	**cmds;

	check_argc(argc);
	init_pipex(&pipex);
	pipex.infile = open(argv[1], O_RDONLY);
	if (pipex.infile < 0)
		perror("Error opening infile");
	pipex.outfile = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (pipex.outfile < 0)
	{
		perror("Error opening/creating outfile");
		pipex.filestatus = 1;
	}
	cmds = build_cmds(argc, argv);
	pipex.status = picoshell(cmds, envp, pipex);
	cleanup_pipex_fds(pipex);
	free(cmds);
	if (pipex.filestatus)
		return (1);
	return (pipex.status);
}
