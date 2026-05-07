/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_chain_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 17:31:03 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/07 11:09:35 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <sys/wait.h>

static void	dup_nextfd(int nextfd, char **cmds)
{
	if (nextfd == -1)
		return ;
	if (dup2(nextfd, STDIN_FILENO) == -1)
	{
		free(cmds);
		exit(1);
	}
	close(nextfd);
}

static int	pipe_and_fork(char **cmds, int i, int pipefd[], pid_t *pid)
{
	if (cmds[i + 1] != NULL)
	{
		if (pipe(pipefd) == -1)
			return (1);
	}
	*pid = fork();
	if (*pid == -1)
	{
		if (cmds[i + 1])
		{
			close(pipefd[0]);
			close(pipefd[1]);
		}
		return (1);
	}
	return (0);
}

static void	clean_fds(int *nextfd, int pipefd[], char *next_cmd)
{
	if (*nextfd != -1)
		close(*nextfd);
	if (next_cmd)
	{
		close(pipefd[1]);
		*nextfd = pipefd[0];
	}
}

int	exec_chain(char **cmds, char **envp, t_pipex pipex)
{
	int		pipefd[2];
	int		nextfd;
	int		i;
	pid_t	pid;

	nextfd = -1;
	i = -1;
	while (cmds[++i])
	{
		if (pipe_and_fork(cmds, i, pipefd, &pid))
			return (1);
		if (pid == 0)
		{
			dup_nextfd(nextfd, cmds);
			dup_pipefds(cmds, i, pipex, pipefd);
			exec_cmd(i, cmds, envp);
		}
		waitpid(pid, &pipex.status, 0);
		clean_fds(&nextfd, pipefd, cmds[i + 1]);
	}
	return (WEXITSTATUS(pipex.status));
}
