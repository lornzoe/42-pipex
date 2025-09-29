/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:19:41 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 04:20:22 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>

static void print_usage_and_exit(void)
{
	ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile\n", STDERR_FILENO);
	exit(1);
}

int initialise_files(int *infile, int *outfile, char **argv, int argc)
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

int initialise_pipe(int pipefd[2], int infile, int outfile)
{
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		close(infile);
		close(outfile);
		return (-1);
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	int files[2];
	int fileerror;
	int pipefd[2];
	int status[2];
	pid_t pid[2];

	if (argc != 5)
		print_usage_and_exit();
	fileerror = initialise_files(&files[INFILE], &files[OUTFILE], argv, argc);
	if (initialise_pipe(pipefd, files[INFILE], files[OUTFILE]) != 0)
		return (1);

	pid[0] = fork();
	if (pid[0] < 0)
	{
		perror("Error forking first child");
		if (files[INFILE] >= 0)
			close(files[INFILE]);
		close(files[OUTFILE]);
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (pid[0] == 0)
	{
		close(pipefd[0]);
		if (files[INFILE] >= 0)
		{
			if (dup2(files[INFILE], STDIN_FILENO) == -1)
			{
				perror("Error duplicating file descriptors");
				close(files[INFILE]);
				close(files[OUTFILE]);
				close(pipefd[1]);
				exit (1);
			}
		}
		else
		{
			if (files[OUTFILE] >= 0)
				close(files[OUTFILE]);
			close(pipefd[1]);
			exit(1);
		}
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			if (files[INFILE] >= 0)
				close(files[INFILE]);
			close(files[OUTFILE]);
			close(pipefd[1]);
			exit (1);
		}
		if (files[INFILE] >= 0)
			close(files[INFILE]);
		close(files[OUTFILE]);
		close(pipefd[1]);
		char **args1 = parse_command(argv[2], envp);
		if (args1 != NULL)
		{
			if (execve(args1[0], args1, envp) == -1)
			{
				perror("execve");
				free_split(args1);
				exit(1);
			}
			free_split(args1);
		}
		else
		{
			ft_putstr_fd("Command not found: ", STDERR_FILENO);
			ft_putstr_fd(argv[2], STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
			exit(127);
		}
	}

	pid[1] = fork();
	if (pid[1] < 0)
	{
		perror("Error forking second child");
		if (files[INFILE] >= 0)
			close(files[INFILE]);
		if (files[OUTFILE] >= 0)
			close(files[OUTFILE]);
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (pid[1] == 0)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			if (files[INFILE] >= 0)
				close(files[INFILE]);
			if (files[OUTFILE] >= 0)
				close(files[OUTFILE]);
			close(pipefd[0]);
			exit(1);
		}
		if (files[OUTFILE] >= 0)
		{
			if (dup2(files[OUTFILE], STDOUT_FILENO) == -1)
			{
				perror("Error duplicating file descriptors");
				if (files[INFILE] >= 0)
					close(files[INFILE]);
				if (files[OUTFILE] >= 0)
					close(files[OUTFILE]);
				close(pipefd[0]);
				exit(1);
			}
		}

		if (files[INFILE] >= 0)
			close(files[INFILE]);
		if (files[OUTFILE] >= 0)
			close(files[OUTFILE]);
		close(pipefd[0]);
		char **args2 = parse_command(argv[3], envp);
		if (args2 != NULL)
		{
			if (execve(args2[0], args2, envp) == -1)
			{
				perror("execve");
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
			exit(127);
		}
	}


	status[0] = 0;
	status[1] = 0;
	if (files[INFILE] >= 0)
		close(files[INFILE]);
	if (files[OUTFILE] >= 0)
		close(files[OUTFILE]);
	close(pipefd[0]);
	close(pipefd[1]);
	
	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);

	if (fileerror)
		return (1);
	if (WIFEXITED(status[1]))
		return (WEXITSTATUS(status[1]));
	if (WIFEXITED(status[0]) && WEXITSTATUS(status[0]) != 0)
		return (WEXITSTATUS(status[0]));
	return (0);
}
