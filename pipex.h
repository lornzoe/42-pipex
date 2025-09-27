/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:22:43 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/27 13:26:59 by lyanga           ###   ########.fr       */
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

# include "libft.h"


#endif