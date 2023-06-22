/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:24:11 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/15 23:06:54 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void categorize_tokens(t_token *tokens, t_size num_tokens)
{
    t_size  i;

    i = -1;
    while (++i < num_tokens)
    {
        if (ft_strcmp(tokens[i].value, "|") == 0)
            tokens[i].type = TOKEN_PIPE;
        else if (ft_strcmp(tokens[i].value, "<") == 0)
            tokens[i].type = TOKEN_REDIR_IN;
        else if (ft_strcmp(tokens[i].value, ">") == 0)
            tokens[i].type = TOKEN_REDIR_OUT;
        else if (ft_strcmp(tokens[i].value, ">>") == 0)
            tokens[i].type = TOKEN_REDIR_APPEND;
        else if (ft_strcmp(tokens[i].value, "'") == 0)
            tokens[i].type = TOKEN_S_QUOTE;
        else if (ft_strcmp(tokens[i].value, "\"") == 0)
            tokens[i].type = TOKEN_D_QUOTE;
        else if (ft_strcmp(tokens[i].value, "$") == 0)
            tokens[i].type = TOKEN_DOLLAR_SIGN;
        else if (ft_strcmp(tokens[i].value, " ") == 0)
            tokens[i].type = TOKEN_WHITESPACE;
        else if (ft_strncmp(tokens[i].value, "$", 1) == 0)
            tokens[i].type = TOKEN_ENV_VAR;
        else
            tokens[i].type = TOKEN_WORD;
    }
}
