/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:11:47 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/01 16:02:40 by lyanga           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	cleanup_t_parser(t_parser *p)
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

void	init_t_parser(t_parser *p)
{
	p->tokens = NULL;
	p->t_count = 0;
	p->in_quote = S_NORMAL;
	p->token_buf = NULL;
	p->buf_len = 0;
}

int	t_parser_append_char(t_parser *p, char c)
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
