/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:44:17 by lyanga            #+#    #+#             */
/*   Updated: 2025/09/30 01:40:31 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <string.h>

// helper function to free a split array
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

// function to check if command exists
// static char *get_paths(char **envp)
// {
//     int i;

//     for (i = 0; envp[i]; i++)
//     {
//         if (ft_strncmp(envp[i], "PATH=", 5) == 0)
//             return (envp[i] + 5);
//     }
//     return (NULL);
// }

// function to find the full path of a command
char *get_command_path(char *cmd, char **envp)
{
    char *path_env;
    char **paths;
    char *full_path;
    char *temp;
    int i;

    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        else
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
        full_path = ft_strjoin(paths[i], "/");
        temp = full_path;
        full_path = ft_strjoin(full_path, cmd);
        if (access(full_path, X_OK) == 0)
        {
            free_split(paths);
            return (full_path);
        }
        free(temp);
        free(full_path);
    }
    free_split(paths);
    return (NULL);
}

// function to split command input
char **parse_command(char *cmd, char **envp)
{
    char **args;
    char *temp;

    args = ft_split(cmd, ' ');
    if (!args)
    {
        perror("Error parsing command");
        return (NULL);
    }
    temp = args[0];
    args[0] = get_command_path(args[0], envp);
    if (!args[0])
    {
        args[0] = temp; // restore original command for error message
        free_split(args);
        return (NULL);
    }
    free(temp);
    return (args);
}
