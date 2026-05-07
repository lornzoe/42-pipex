/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 19:55:57 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/07 10:42:54 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"
#include "ft_gnl.h"
#include <fcntl.h>
#include <sys/wait.h>

static void fill_heredoc(char *delimiter)
{
	int fd;
	int len;
	char *buffer;

	fd = open(HEREDOC_FILE_LOC, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		exit(1);
	len = ft_strlen(delimiter);
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		buffer = ft_gnl_mandatory(STDIN_FILENO);
		if (buffer == NULL)
					break ;
		if (ft_strncmp(buffer, delimiter, len) == 0 && buffer[len] == '\n')
		{
			free(buffer);
			break ;
		}
		ft_putstr_fd(buffer, fd);
		free(buffer);
		buffer = NULL;
	}
	close(fd);
	exit(0);
}

void make_heredoc(t_pipex *pipex)
{
	pid_t	pid;
	int		status;

	if (!pipex->delimiter)
		return ;
	pid = fork();
	if (pid < 0)
		return ;
	if (pid == 0)
		fill_heredoc(pipex->delimiter);
	waitpid(pid, &status, 0);
	return ;
}
