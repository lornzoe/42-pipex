/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:19:41 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 03:29:58 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv, char **envp)
{
	int fileerror;
	fileerror = 0;
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile", STDERR_FILENO);
		return (1);
	}

	int infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		perror("Error opening infile");
	}
	
	int outfile = open(argv[argc - 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		perror("Error opening/creating outfile");
		close(infile);
		fileerror = 1; 
	}

	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		close(infile);
		close(outfile);
		return (0);
	}

	pid_t pid1 = fork();
	if (pid1 < 0)
	{
		perror("Error forking first child");
		close(infile);
		close(outfile);
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (pid1 == 0)
	{
		close(pipefd[0]);
		if (infile >= 0)
		{
			if (dup2(infile, STDIN_FILENO) == -1)
			{
				perror("Error duplicating file descriptors");
				close(infile);
				close(outfile);
				close(pipefd[1]);
				exit (1);
			}
		}
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close(infile);
			close(outfile);
			close(pipefd[1]);
			exit (1);
		}
		close(infile);
		close(outfile);
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

	pid_t pid2 = fork();
	if (pid2 < 0)
	{
		perror("Error forking second child");
		close(infile);
		close(outfile);
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (pid2 == 0)
	{
		close(pipefd[1]);
		if (outfile >= 0)
		{
			if (dup2(outfile, STDOUT_FILENO) == -1)
			{
				perror("Error duplicating file descriptors");
				close(infile);
				close(outfile);
				close(pipefd[0]);
				exit(1);
			}
		}
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close(infile);
			close(outfile);
			close(pipefd[0]);
			exit(1);
		}
		close(infile);
		close(outfile);
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
	int status1;
	int status2;

	status1 = 0;
	status2 = 0;
	if (infile >= 0)
		close(infile);
	if (outfile >= 0)
		close(outfile);
	close(pipefd[0]);
	close(pipefd[1]);
	
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);

	if (fileerror)
		return (1);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	if (WIFEXITED(status1) && WEXITSTATUS(status1) != 0)
		return (WEXITSTATUS(status1));
	return (0);
}
