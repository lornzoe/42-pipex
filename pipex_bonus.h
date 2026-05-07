/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:22:43 by lyanga            #+#    #+#             */
/*   Updated: 2026/05/07 11:08:42 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft.h"

typedef struct s_pipex
{
	int		has_heredoc;
	char	*delimiter;
	int		infile;
	char	*infile_loc;
	int		outfile;
	int		outfile_flags;
	int		filestatus;
	int		status;
}	t_pipex;

// cmd_split.c
// - state definitions
# define S_NORMAL 0
# define S_SINGLE 1
# define S_DOUBLE 2

// - struct for parser
typedef struct s_parser
{
	char	**tokens;
	int		t_count;
	int		in_quote;
	char	*token_buf;
	size_t	buf_len;
}	t_parser;

char	**cmd_split(char *str);
// - helper funcs
void	cleanup_t_parser(t_parser *p);
void	init_t_parser(t_parser *p);
int		t_parser_append_char(t_parser *p, char c);

// ft_realloc.c
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

// util.c
void	free_split(char **split);

// path.c
char	*get_command_path(char *cmd, char **envp);

// parse.c
char	**parse_command(char *cmd, char **envp);

// exec_chain.c
int		exec_chain(char **cmds, char **envp, t_pipex pipex);
// exec_chain_2.c
void	exec_cmd(int i, char **cmds, char **envp);
void	dup_pipefds(char **cmds, int i, t_pipex pipex, int pipefd[]);

// heredoc.c
# ifndef HEREDOC_FILE_LOC 
#  define HEREDOC_FILE_LOC "/tmp/heredoc_pipex_lyanga"
# endif

void	make_heredoc(t_pipex *pipex);

#endif