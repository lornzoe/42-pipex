/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 15:16:12 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/05 16:18:41 by lyanga           ###   ########.fr       */
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

int    picoshell(char **cmds, char **envp, t_pipex pipex)
{
	int pipefd[2];
	int nextfd = -1;
	int i = 0;
	pid_t pid;

	while (cmds[i])
	{
		if (cmds[i + 1] != NULL)
		{
			if (pipe(pipefd) == -1)
				return 1;
		}
		pid = fork();
		if (pid == -1)
		{
			if (cmds[i+1])
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return 1;
		}
		if (pid == 0)
		{ // inside child process
			if (nextfd != -1)
			{
				if (dup2(nextfd, STDIN_FILENO) == -1)
					exit(1);
				close(nextfd);
			}
			if (cmds[i+1])
			{
				if (i == 0 && pipex.infile >= 0)
				{
					if (dup2(pipex.infile, STDIN_FILENO) == -1)
						exit(1);
					close(pipex.infile);
				}
				close(pipefd[0]);
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(pipefd[1]);
			}
			else if (pipex.outfile >= 0)
			{
				if (dup2(pipex.outfile, STDOUT_FILENO) == -1)
					exit(1);
				close(pipex.outfile);
			}

			char **args = parse_command(cmds[i], envp);
			if (args != NULL)
			{
				execve(args[0], args, envp);
				exit(1); // if execvp fails
			}
		}
		if (nextfd != -1)
			close(nextfd);
		if (cmds[i+1])
		{
			close(pipefd[1]);
			nextfd = pipefd[0];
		}
		i++;
	}
	waitpid(pid, &pipex.status, 0);
	return WEXITSTATUS(pipex.status);
}

static void init_pipex(t_pipex *pipex)
{
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->status = 0;
	pipex->filestatus = 0;
}

static void cleanup_pipex_fds(t_pipex pipex)
{
	if (pipex.infile >= 0)
		close(pipex.infile);
	pipex.infile = -1;
	if (pipex.outfile >= 0)
		close(pipex.outfile);
	pipex.outfile = -1;
}

static char** build_cmds(int argc, char **argv)
{
	char **cmds;
	int i;

	cmds = malloc(sizeof(char *) * (argc - 2));
	i = 0;
	while (i < argc - 3)
	{
		cmds[i] = argv[i + 2];
		i++;
	}
	cmds[i] = NULL;
	return cmds;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	char **cmds;

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
