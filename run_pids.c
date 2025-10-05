/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pids.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 05:21:51 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/05 17:25:35 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdlib.h>

static void	setup_pid0(int pipefd[2], int files[2], int fdstokill[2])
{
	close(pipefd[0]);
	if (files[INFILE] >= 0)
	{
		if (dup2(files[INFILE], STDIN_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close_3(files[INFILE], files[OUTFILE], pipefd[1]);
			exit (1);
		}
	}
	else
	{
		close_2(files[OUTFILE], pipefd[1]);
		exit(1);
	}
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("Error duplicating file descriptors");
		close_3(files[INFILE], files[OUTFILE], pipefd[1]);
		exit (1);
	}
	fdstokill[0] = STDIN_FILENO;
	fdstokill[1] = STDOUT_FILENO;
	close_3(files[INFILE], files[OUTFILE], pipefd[1]);
}

static void	handle_pid0(char **argv, char **envp, int fdstokill[2])
{
	char	**args1;

	args1 = parse_command(argv[2], envp);
	if (args1 != NULL)
	{
		if (execve(args1[0], args1, envp) == -1)
		{
			perror("execve failed");
			free_split(args1);
			close_2(fdstokill[0], fdstokill[1]);
			exit(1);
		}
		free_split(args1);
	}
	else
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putstr_fd(argv[2], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		close_2(fdstokill[0], fdstokill[1]);
		free_split(args1);
		exit(127);
	}
}

static void	setup_pid1(int pipefd[2], int files[2], int fdstokill[2])
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("Error duplicating file descriptors");
		close_3(files[INFILE], files[OUTFILE], pipefd[0]);
		exit(1);
	}
	if (files[OUTFILE] >= 0)
	{
		if (dup2(files[OUTFILE], STDOUT_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close_3(files[INFILE], files[OUTFILE], pipefd[0]);
			exit(1);
		}
	}
	fdstokill[0] = STDIN_FILENO;
	fdstokill[1] = STDOUT_FILENO;
	close_3(files[INFILE], files[OUTFILE], pipefd[0]);
}

static void	handle_pid1(char **argv, char **envp, int fdstokill[2])
{
	char	**args2;

	args2 = parse_command(argv[3], envp);
	if (args2 != NULL)
	{
		if (execve(args2[0], args2, envp) == -1)
		{
			ft_putstr_fd("Command failed\n", STDERR_FILENO);
			perror("execve failed");
			close_2(fdstokill[0], fdstokill[1]);
			free_split(args2);
			exit(1);
		}
		free_split(args2);
	}
	else
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putstr_fd(argv[3], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		close_2(fdstokill[0], fdstokill[1]);
		free_split(args2);
		exit(127);
	}
}

int	run_pids(int files[3], int pipefd[2], char **argv, char **envp)
{
	pid_t	pid[2];
	int		status[2];
	int		fdstoclose[2];

	initialise_fork(&pid[0], files, pipefd);
	if (pid[0] == 0)
	{
		setup_pid0(pipefd, files, fdstoclose);
		handle_pid0(argv, envp, fdstoclose);
	}
	initialise_fork(&pid[1], files, pipefd);
	if (pid[1] == 0)
	{
		if (files[FILEERROR_CHECK])
		{
			close_3(files[INFILE], pipefd[0], pipefd[1]);
			exit(1);
		}
		setup_pid1(pipefd, files, fdstoclose);
		handle_pid1(argv, envp, fdstoclose);
	}
	close_4(files[INFILE], files[OUTFILE], pipefd[0], pipefd[1]);
	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);
	return (WEXITSTATUS(status[1]));
}
