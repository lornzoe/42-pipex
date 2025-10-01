/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:44:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/01 11:19:14 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>
#include <unistd.h>

// static char	**split_attempt(char *cmd)
// {
// 	char	**args;

// 	args = cmd_split(cmd);
// 	if (!args || !args[0])
// 	{
// 		if (args)
// 			free_split(args);
// 		return (ft_split(cmd, ' '));
// 	}
// 	return (args);
// }

char	**parse_command(char *cmd, char **envp)
{
	char	**args;
	char	*temp;

	// ft_putstr_fd(cmd, STDERR_FILENO);
	// ft_putstr_fd("\n", STDERR_FILENO);

	if (!cmd || !*cmd)
		return (NULL);
	args = cmd_split(cmd); //split_attempt(cmd);
	if (!args || !args[0])
	{
		if (args)
			free_split(args);
		return (NULL);
	}

	int i = 0;
	ft_putstr_fd("Tokens:\n", STDERR_FILENO);
    while(args[i] != NULL)
	{
		ft_putstr_fd(">", STDERR_FILENO);
		ft_putstr_fd(args[i++], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
    fflush(NULL);
	temp = args[0];
	args[0] = get_command_path(args[0], envp);
	if (!args[0])
	{
		free(temp);
		free_split(args);
		return (NULL);
	}
	free(temp);
	return (args);
}
