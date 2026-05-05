/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_chain.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:31:03 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/05 18:57:08 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void dup_nextfd(int nextfd, char **cmds)
{
	if (nextfd != -1)
	{
		if (dup2(nextfd, STDIN_FILENO) == -1)
		{
			free(cmds);
			exit(1);
		}
		close(nextfd);
	}
}

static void dup_pipefds(char **cmds, int i, t_pipex pipex, int pipefd[])
{
	if (cmds[i + 1])
	{
		if (i == 0)
		{
			if (pipex.infile >= 0)
			{
				if (dup2(pipex.infile, STDIN_FILENO) == -1)
				{
					free(cmds);
					exit(1);
				}
				close(pipex.infile);
			}
			else
			{
				free(cmds);
				exit(1);
			}
		} 
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			free(cmds);
			exit(1);
		}
		close(pipefd[1]);
	}
	else if (!cmds[i + 1] && pipex.filestatus == 1)
	{
		if (pipex.infile >= 0)
			close(pipex.infile);
		if (pipex.outfile >= 0)
			close(pipex.outfile);
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		free(cmds);
		exit(1);
	}
	else if (pipex.outfile >= 0)
	{
		if (dup2(pipex.outfile, STDOUT_FILENO) == -1)
		{
			free(cmds);
			exit(1);
		}
		close(pipex.outfile);
	}
}

static void exec_cmd(int i, char **cmds, char **envp)
{
	char **args;
	
	args = parse_command(cmds[i], envp);
	if (args != NULL)
		execve(args[0], args, envp);
	close(STDIN_FILENO);
	close (STDOUT_FILENO);
	free(cmds);
	if (args != NULL)
		exit(1);
	exit(127);
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
			if (cmds[i + 1])
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return 1;
		}
		if (pid == 0)
		{ // inside child process
			if (nextfd != -1)
				dup_nextfd(nextfd, cmds);
			dup_pipefds(cmds, i, pipex, pipefd);
			exec_cmd(i, cmds, envp);
		}
		waitpid(pid, &pipex.status, 0);
		if (nextfd != -1)
			close(nextfd);
		if (cmds[i + 1])
		{
			close(pipefd[1]);
			nextfd = pipefd[0];
		}
		i++;
	}
	return WEXITSTATUS(pipex.status);
}
