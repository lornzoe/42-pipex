/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 15:57:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/27 17:19:51 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <unistd.h>

int main(int argc, char **argv, char **envp)
{
    char *const arg[] = {"ls", "asdf", NULL};
    
    printf("test output\n");

    int x = execve("/usr/bin/ls", arg, envp);
    perror("execve");
    
    printf("output of x: %d\n", x);

    return 0;
}