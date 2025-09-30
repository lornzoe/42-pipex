/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:22:43 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/01 06:58:13 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h> 

# include "libft.h"

# define INFILE 0
# define OUTFILE 1

// util.c
void	free_split(char **split);

// ft_realloc.c
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

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