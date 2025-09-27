/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:19:41 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/27 13:50:31 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main (int argc, char **argv, char **envp)
{
	// 1. Check argument count (should be 5: ./pipex infile cmd1 cmd2 outfile)
    //    - If not, print error and exit.
	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 outfile", STDERR_FILENO);
		return (1);
	}

    // 2. Open infile for reading.
    //    - If fail, print error and exit.
	int infile = open(argv[1], O_RDONLY);
	if (infile < 0)
	{
		perror("Error opening infile");
		return (1);
	}

    // 3. Open outfile for writing (create/truncate).
    //    - If fail, print error and exit.
	int outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (outfile < 0)
	{
		perror("Error opening/creating outfile");
		close(infile);
		return (1);
	}

    // 4. Create a pipe (int pipefd[2]).
    //    - If fail, print error and exit.
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("Error creating pipe");
		close(infile);
		close(outfile);
		return (1);
	}

    // 5. Fork the first child process.
    //    - In child 1:
    //      a. Redirect infile to STDIN (dup2).
    //      b. Redirect pipe write end to STDOUT (dup2).
    //      c. Close unused pipe ends.
    //      d. Parse cmd1 and its arguments.
    //      e. Find cmd1 path using envp.
    //      f. execve(cmd1, args, envp).
    //      g. If execve fails, print error and exit.
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
		// Child 1 process
		if (dup2(infile, STDIN_FILENO) == -1 ||
		dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close(infile);
			close(outfile);
			close(pipefd[0]);
			close(pipefd[1]);
			return (1);
		}
	}

    // 6. Fork the second child process.
    //    - In child 2:
    //      a. Redirect pipe read end to STDIN (dup2).
    //      b. Redirect outfile to STDOUT (dup2).

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
		// Child 2 process
		if (dup2(pipefd[0], STDIN_FILENO) == -1 ||
		dup2(outfile, STDOUT_FILENO) == -1)
		{
			perror("Error duplicating file descriptors");
			close(infile);
			close(outfile);
			close(pipefd[0]);
			close(pipefd[1]);
			return (1);
		}
	}
	//      c. Close unused pipe ends.
    //      d. Parse cmd2 and its arguments.
    //      e. Find cmd2 path using envp.
    //      f. execve(cmd2, args, envp).
    //      g. If execve fails, print error and exit.

    // 7. In parent:
    //    - Close all file descriptors (infile, outfile, pipe ends).
    //    - Wait for both children to finish.
	
	close(infile);
	close(outfile);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

    // 8. Exit.
	return (0);
}