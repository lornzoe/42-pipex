/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:44:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/01 17:18:53 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex.h"
#include <stdio.h>
#include <unistd.h>

static char	**split_attempt(char *cmd)
{
	char	**args;

	args = cmd_split(cmd);
	if (!args || !args[0])
	{
		if (args)
			free_split(args);
		return (ft_split(cmd, ' '));
	}
	return (args);
}

char	**parse_command(char *cmd, char **envp)
{
	char	**args;
	char	*temp;

	if (!cmd || !*cmd)
		return (NULL);
	args = split_attempt(cmd);
	if (!args || !args[0])
	{
		if (args)
			free_split(args);
		return (NULL);
	}
	temp = args[0];
	args[0] = get_command_path(args[0], envp);
	if (!args[0])
	{
		args[0] = temp;
		free_split(args);
		return (NULL);
	}
	free(temp);
	return (args);
}
