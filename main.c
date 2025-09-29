/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:19:41 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 05:12:25 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void check_argc(int argc)
{
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile\n",
			STDERR_FILENO);
		exit(1);
	}
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

// varadic function to handle closing multiple fds
void close_fds(int count, ...)
{
	va_list args;
	int fd;

	va_start(args, count);
	for (int i = 0; i < count; i++)
	{
		fd = va_arg(args, int);
		if (fd >= 0)
			close(fd);
	}
	va_end(args);
}

int initialise_fork(int *pid, int files[2], int pipefd[2])
{
	*pid = fork();
	if (*pid < 0)
	{
		perror("Error forking process");
		close_fds(4, files[INFILE], files[OUTFILE], pipefd[0], pipefd[1]);
		exit(1);
	}
	return (0);
}

void setup_pid0(int files[2], int pipefd[2])
{
	close(pipefd[0]);
	if (files[INFILE] >= 0)
	{
		if (dup2(files[INFILE], STDIN_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close_fds(3, files[INFILE], files[OUTFILE], pipefd[1]);
			exit (1);
		}
	}
	else
	{
		close_fds(2, files[OUTFILE], pipefd[1]);
		exit(1);
	}
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("Error duplicating file descriptors");
		close_fds(3, files[INFILE], files[OUTFILE], pipefd[1]);
		exit (1);
	}
	close_fds(3, files[INFILE], files[OUTFILE], pipefd[1]);
}

void handle_pid0(char **argv, char **envp)
{
	char	**args1;

	args1 = parse_command(argv[2], envp);
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

void setup_pid1(int pipefd[2], int files[2])
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("Error duplicating file descriptors");
		close_fds(3, files[INFILE], files[OUTFILE], pipefd[0]);
		exit(1);
	}
	if (files[OUTFILE] >= 0)
	{
		if (dup2(files[OUTFILE], STDOUT_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close_fds(3, files[INFILE], files[OUTFILE], pipefd[0]);
			exit(1);
		}
	}
	close_fds(3, files[INFILE], files[OUTFILE], pipefd[0]);
}

void handle_pid1(char **argv, char **envp)
{
	char	**args2;

	args2 = parse_command(argv[3], envp);
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

int main(int argc, char **argv, char **envp)
{
	int		files[2];
	int		fileerror;
	int		pipefd[2];
	int		status[2];
	pid_t	pid[2];

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile\n",
			STDERR_FILENO);
		return (1);
	}
	fileerror = initialise_files(&files[INFILE], &files[OUTFILE], argv, argc);
	if (initialise_pipe(pipefd, files[INFILE], files[OUTFILE]) != 0)
		return (1);
	initialise_fork(&pid[0], files, pipefd);
	if (pid[0] == 0)
	{
		setup_pid0(files, pipefd);
		handle_pid0(argv, envp);
	}
	initialise_fork(&pid[1], files, pipefd);
	if (pid[1] == 0)
	{
		setup_pid1(pipefd, files);
		handle_pid1(argv, envp);
	}
	close_fds(4, files[INFILE], files[OUTFILE], pipefd[0], pipefd[1]);
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
