/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_chain_2_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 19:22:48 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/05 20:06:18 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"
#include <stdlib.h>
#include <unistd.h>

void	exec_cmd(int i, char **cmds, char **envp)
{
	char	**args;

	args = parse_command(cmds[i], envp);
	if (args != NULL)
		execve(args[0], args, envp);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	free(cmds);
	if (args != NULL)
		exit(1);
	exit(127);
}

static void	handle_last_cases(char **cmds, t_pipex pipex)
{
	if (pipex.filestatus == 1)
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
	if (pipex.outfile >= 0)
	{
		if (dup2(pipex.outfile, STDOUT_FILENO) == -1)
		{
			free(cmds);
			exit(1);
		}
		close(pipex.outfile);
	}
}

void	dup_pipefds(char **cmds, int i, t_pipex pipex, int pipefd[])
{
	if (cmds[i + 1])
	{
		if (i == 0)
		{
			if (pipex.infile >= 0 && dup2(pipex.infile, STDIN_FILENO) != -1)
				close(pipex.infile);
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
	else
		handle_last_cases(cmds, pipex);
}
