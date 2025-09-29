/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:22:43 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 07:21:10 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>     // fork, pipe, dup2, execve, access, close, read, write
# include <stdlib.h>     // malloc, free, exit
# include <stdio.h>      // perror
# include <fcntl.h>      // open
# include <sys/wait.h>   // wait, waitpid
# include <string.h>     // strerror (if needed)
# include <stdarg.h>    

# include "libft.h"

# define INFILE 0
# define OUTFILE 1

// util.c
void	free_split(char **split);

// path.c
char	*get_command_path(char *cmd, char **envp);

//parse.c
char	**parse_command(char *cmd, char **envp);

// close.c
void	close_2(int fd1, int fd2);
void	close_3(int fd1, int fd2, int fd3);
void	close_4(int fd1, int fd2, int fd3, int fd4);

// initialise.c
int		initialise_files(int *infile, int *outfile, char **argv, int argc);
int		initialise_pipe(int pipefd[2], int infile, int outfile);
int		initialise_fork(int *pid, int files[2], int pipefd[2]);

// run_pids.c
int		run_pids(int files[2], int pipefd[2], char **argv, char **envp);

#endif