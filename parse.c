/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:44:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 02:07:12 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void free_split(char **split)
{
	int i;

	if (!split)
		return;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char *get_command_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = NULL;
	for (i = 0; envp[i]; i++)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break;
		}
	}
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	for (i = 0; paths[i] != NULL; i++)
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
		{
			free_split(paths);
			return (NULL);
		}
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
		{
			free_split(paths);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free_split(paths);
			return (full_path);
		}
		free(full_path);
	}
	free_split(paths);
	return (NULL);
}

char **parse_command(char *cmd, char **envp)
{
	char	**args;
	char	*temp;

	if (!cmd || !*cmd)
		return (NULL);
	args = ft_split(cmd, ' ');
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
		free(temp);
		free_split(args);
		return (NULL);
	}
	free(temp);
	return (args);
}
