/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:00:41 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/12 22:42:55 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *tokenize_input(char *input, t_size alloced, t_size *num_tokens)
{
	t_token				*tokens;
	t_token				*new_token;
	t_token_state		state;
	const t_cmp			cmp_func[] = { is_meta, is_squote, is_dquote, is_space };

	if (!init_tokenizer(input, &tokens, &alloced, &state))
		return (set_err_token(tokens, num_tokens, MALLOC_ERR));
	while (*input)
	{
		new_token = tokenize(&input, cmp_func[state], (int) state, &state);
		if (!new_token)
			return (set_err_token(tokens, num_tokens, MALLOC_ERR));
		tokens[(*num_tokens)++] = *new_token;
		free(new_token);
		if (tokens[*num_tokens - 1].type == TOKEN_ERROR)
			break ;
		else if (tokens[*num_tokens - 1].type == TOKEN_WHITESPACE)
		{
			free(tokens[(*num_tokens) - 1].val);
			tokens[(*num_tokens)-- - 1].val = 0;
		}
		tokens = should_realloc(tokens, num_tokens, &alloced);
		if (!tokens)
			return (set_err_token(tokens, num_tokens, MALLOC_ERR));
	}
	if (!check_parenthesis(tokens, *num_tokens))
		tokens = set_err_token(tokens, num_tokens, PAREN_NOT_CLOSED);
	return (tokens);
}
