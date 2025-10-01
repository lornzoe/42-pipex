/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:16:29 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/01 13:59:35 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <stdio.h>

static int	ft_finalize_token(t_parser *p)
{
	char	**new_tokens;
	size_t	old_size;
	size_t	new_size;

	if (p->token_buf == NULL)
		return (1);
	old_size = p->t_count * sizeof(char *);
	new_size = (p->t_count + 1) * sizeof(char *);
	new_tokens = (char **)ft_realloc(p->tokens, old_size, new_size);
	if (new_tokens == NULL)
		return (0);
	p->tokens = new_tokens;
	p->tokens[p->t_count] = p->token_buf;
	(p->t_count)++;
	p->token_buf = NULL;
	p->buf_len = 0;
	return (1);
}

static int	ft_handle_normal(t_parser *p, char c, const char **s)
{
	if (ft_isspace(c))
	{
		if (!ft_finalize_token(p))
			return (0);
		while (ft_isspace(**s))
			(*s)++;
		return (2);
	}
	else if (c == '\'' || c == '\"')
	{
		// if (!t_parser_append_char(p, c)) // Append the opening quote
		// return (0);
		if (c == '\'')
			p->in_quote = S_SINGLE;
		else
			p->in_quote = S_DOUBLE;
	}
	else if (!t_parser_append_char(p, c))
		return (0);
	return (1);
}

static char	**ft_finalize_result(t_parser *p)
{
	char	**final_tokens;
	size_t	old_size;
	size_t	new_size;

	if (p->in_quote != S_NORMAL || !ft_finalize_token(p))
	{
		fprintf(stderr, "Error: Parsing failed or unmatched quote.\n");
		cleanup_t_parser(p);
		return (NULL);
	}
	old_size = p->t_count * sizeof(char *);
	new_size = (p->t_count + 1) * sizeof(char *);
	final_tokens = (char **)ft_realloc(p->tokens, old_size, new_size);
	if (final_tokens == NULL)
	{
		cleanup_t_parser(p);
		return (NULL);
	}
	final_tokens[p->t_count] = NULL; 
	return (final_tokens);
}

static int handle_quotes(t_parser *p, char c)
{
	if (c == '\'' && p->in_quote == S_SINGLE)
		p->in_quote = S_NORMAL;
	else if (c == '\"' && p->in_quote == S_DOUBLE)
		p->in_quote = S_NORMAL;
	else
	{
		if (!t_parser_append_char(p, c))
			return (-1);
	}
	return (0);
}

char	**cmd_split(char *str)
{
	t_parser	p;
	const char	*s;
	int			result;

	s = str;
	init_t_parser(&p);
	if (str == NULL)
		return (NULL);
	while (*s)
	{
		if (p.in_quote == S_NORMAL)
		{
			result = ft_handle_normal(&p, *s, &s);
			if (result == 0)
				break ;
			if (result == 2)
				continue ;
		}
		else if (handle_quotes(&p, *s))
			break;
		s++;
	}
	return (ft_finalize_result(&p));
}

// int main() {
// 	char* command_string = "grep -e \'make -C\'";
//     int count;
//     char** args = cmd_split(command_string);

//     printf("Tokens:\n");
// 	int i = 0;
// 	if (args == NULL)
// 		return (0);
//     while(args[i] != NULL)
// 	{
//         printf("  %s\n", args[i++]);
//     }
//     i = 0;
// 	while (args[i])
// 	{
// 		free(args[i]);
// 		i++;
// 	}
// 	free(args);
//     return 0;
// }