/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 16:16:29 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/01 07:39:04 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <stdio.h>

// --- State Definitions ---
#define S_NORMAL 0
#define S_SINGLE 1
#define S_DOUBLE 2

// --- Data Structure for Parser State ---
typedef struct s_parser
{
	char	**tokens;
	int		t_count;
	int		in_quote;
	char	*token_buf;
	size_t	buf_len;
}				t_parser;

// Assumed prototype for custom realloc (must be linked from ft_realloc.c)
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);

static void	ft_cleanup_shlex(t_parser *p)
{
	int	i;

	i = 0;
	if (p->token_buf)
		free(p->token_buf);
	if (p->tokens)
	{
		while (i < p->t_count)
		{
			free(p->tokens[i]);
			i++;
		}
		free(p->tokens);
	}
}

static int	ft_append_char(t_parser *p, char c)
{
	char	*new_token;
	size_t	old_len;
	size_t	new_len;

	old_len = p->buf_len + 1;
	new_len = p->buf_len + 2;
	new_token = (char *)ft_realloc(p->token_buf, old_len, new_len);
	if (new_token == NULL)
		return (0);
	p->token_buf = new_token;
	(p->token_buf)[p->buf_len] = c;
	(p->buf_len)++;
	(p->token_buf)[p->buf_len] = '\0';
	return (1);
}

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

// MODIFIED: Append the quote character before changing state.
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
		if (!ft_append_char(p, c)) // Append the opening quote
			return (0);
		if (c == '\'')
			p->in_quote = S_SINGLE;
		else
			p->in_quote = S_DOUBLE;
	}
	else
	{
		if (!ft_append_char(p, c))
			return (0);
	}
	return (1);
}

// No longer needed since logic is merged into main loop for Norm compliance.
// static int	ft_handle_quoted(...)

static char	**ft_finalize_result(t_parser *p)
{
	char	**final_tokens;
	size_t	old_size;
	size_t	new_size;

	if (p->in_quote != S_NORMAL || !ft_finalize_token(p))
	{
		fprintf(stderr, "Error: Parsing failed or unmatched quote.\n");
		ft_cleanup_shlex(p);
		return (NULL);
	}
	old_size = p->t_count * sizeof(char *);
	new_size = (p->t_count + 1) * sizeof(char *);
	final_tokens = (char **)ft_realloc(p->tokens, old_size, new_size);
	if (final_tokens == NULL)
	{
		ft_cleanup_shlex(p);
		return (NULL);
	}
	final_tokens[p->t_count] = NULL; 
	return (final_tokens);
}

// MODIFIED: Logic simplified to append all characters in a quoted state.
char	**cmd_split(char *str)
{
	t_parser	p;
	const char	*s;
	int			result;

	s = str;
	p.tokens = NULL;
	p.t_count = 0;
	p.in_quote = S_NORMAL;
	p.token_buf = NULL;
	p.buf_len = 0;
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
		else // In quoted state
		{
			if (*s == '\'' && p.in_quote == S_SINGLE)
				p.in_quote = S_NORMAL;
			else if (*s == '\"' && p.in_quote == S_DOUBLE)
				p.in_quote = S_NORMAL;
			
			// Append character regardless of whether it's the closing quote or not
			if (!ft_append_char(&p, *s)) 
				break ;
		}
		s++;
	}
	return (ft_finalize_result(&p));
}


int main() {
	char* command_string = "this is 'my string' -A\"aaaa\" that --has=arguments";
    int count;
    char** args = cmd_split(command_string);

    printf("Tokens:\n");
	int i = 0;
    while(args[i] != NULL)
	{
        printf("  %s\n", args[i++]);
    }
    i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
    return 0;
}