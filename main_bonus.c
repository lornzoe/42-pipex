/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 15:16:12 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/07 18:54:43 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "pipex_bonus.h"

static void	check_argc(int argc, char **argv)
{
	if ((argc < 6 && argc >= 2 && ft_strncmp(argv[1], "here_doc", 9) == 0)
		|| argc < 5)
	{
		ft_putstr_fd("Usage: ./pipex_bonus infile cmd1 cmd2 [cmd3 ...] \
			outfile\n",
			STDERR_FILENO);
		ft_putstr_fd("OR: ./pipex_bonus here_doc DELIMITER cmd1 cmd2 [cmd3 ...] \
			outfile\n",
			STDERR_FILENO);
		exit(1);
	}
}

static void	init_pipex(t_pipex *pipex, int argc, char **argv)
{
	if (argc > 2 && ft_strncmp(argv[1], "here_doc", 9) == 0)
	{
		pipex->has_heredoc = 1;
		pipex->delimiter = argv[2];
		make_heredoc(pipex);
		pipex->infile_loc = HEREDOC_FILE_LOC;
		pipex->outfile_flags = O_WRONLY | O_CREAT | O_APPEND;
	}
	else
	{
		pipex->has_heredoc = 0;
		pipex->infile_loc = argv[1];
		pipex->outfile_flags = O_CREAT | O_WRONLY | O_TRUNC;
	}
	pipex->infile = -1;
	pipex->outfile = -1;
	pipex->status = 0;
	pipex->filestatus = 0;
}

static void	cleanup_pipex_fds(t_pipex pipex)
{
	if (pipex.infile >= 0)
		close(pipex.infile);
	pipex.infile = -1;
	if (pipex.outfile >= 0)
		close(pipex.outfile);
	pipex.outfile = -1;
}

static char	**build_cmds(int argc, char **argv, int offset)
{
	char	**cmds;
	int		i;

	cmds = malloc(sizeof(char *) * (argc - 2));
	i = 0;
	while (i < argc - offset - 1)
	{
		cmds[i] = argv[i + offset];
		i++;
	}
	cmds[i] = NULL;
	return (cmds);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	char	**cmds;

	check_argc(argc, argv);
	init_pipex(&pipex, argc, argv);
	pipex.infile = open(pipex.infile_loc, O_RDONLY);
	if (pipex.infile < 0)
		perror("Error opening infile");
	pipex.outfile = open(argv[argc - 1], pipex.outfile_flags, 0644);
	if (pipex.outfile < 0)
	{
		perror("Error opening/creating outfile");
		pipex.filestatus = 1;
	}
	cmds = build_cmds(argc, argv, 2 + pipex.has_heredoc);
	pipex.status = exec_chain(cmds, envp, pipex);
	cleanup_pipex_fds(pipex);
	free(cmds);
	if (pipex.filestatus)
		return (1);
	return (pipex.status);
}
