/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:24:11 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/30 13:39:23 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void categorize_tokens(t_token *tokens, t_size num_tokens)
{
	t_size	i;

	i = -1;
	while (++i < num_tokens)
	{
		if (tokens[i].type == TOKEN_OPERATOR)
		{
			if (ft_strcmp(tokens[i].val, "|") == 0)
				tokens[i].type = TOKEN_PIPE;
			else if (ft_strcmp(tokens[i].val, "<") == 0)
				tokens[i].type = TOKEN_REDIR_IN;
			else if (ft_strcmp(tokens[i].val, ">") == 0)
				tokens[i].type = TOKEN_REDIR_OUT;
			else if (ft_strncmp(tokens[i].val, "(", 1) == 0)
				tokens[i].type = TOKEN_L_PAREN;
			else if (ft_strncmp(tokens[i].val, ")", 1) == 0)
				tokens[i].type = TOKEN_R_PAREN;
			else if (ft_strncmp(tokens[i].val, "*", 1) == 0)
				tokens[i].type = TOKEN_WILDCARD;
			else
				tokens[i].type = TOKEN_UNKNOWN;
		}
		else if (tokens[i].type == TOKEN_DQ_STR || tokens[i].type == TOKEN_SQ_STR)
		{
		}
	}
}
