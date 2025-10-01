/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lyanga <lyanga@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 10:11:47 by lyanga            #+#    #+#             */
/*   Updated: 2025/10/01 10:15:34 by lyanga           ###   ########.fr       */
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

void    init_t_parser(t_parser *p)
{
    p->tokens = NULL;
	p->t_count = 0;
	p->in_quote = S_NORMAL;
	p->token_buf = NULL;
	p->buf_len = 0;
}
