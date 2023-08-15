/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_states.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 11:41:30 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 13:06:57 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_state	update_state(char ch)
{
	if (is_squote(ch))
		return (SQUOTE);
	else if (is_dquote(ch))
		return (DQUOTE);
	else if (is_meta(ch) || is_space(ch))
		return (META_CH);
	else if (ch == '\0')
		return (END);
	return (NORMAL);
}

t_token	*tokenize(char **input, t_cmp cmp, t_type type, t_state *state)
{
	t_token	*new_token;

	if (cmp == is_space)
		return (tokenize_meta(input, state));
	else
		new_token = split_input(*input, input, cmp, type);
	if (!new_token)
		return (0);
	*state = update_state(**input);
	return (new_token);
}

t_token	*tokenize_operator(char **input, t_state *state)
{
	t_token	*new_token;

	if (is_dmeta_str(*input))
	{
		new_token = create_token(is_dmeta_str(*input), *input, 2);
		(*input)++;
	}
	else
		new_token = create_token(get_operator_type(**input), *input, 1);
	*state = update_state(*(++(*input)));
	return (new_token);
}

t_token	*tokenize_meta(char **input, t_state *state)
{
	t_token	*new_token;

	if (is_space(**input))
		new_token = tokenize(input, is_not_space, TOKEN_WHITESPACE, state);
	else
		new_token = tokenize_operator(input, state);
	if (!new_token)
		return (0);
	*state = update_state(**input);
	return (new_token);
}

t_bool	init_tokenizer(char *input, t_token **tokens, t_size *alloced, t_state *state)
{
	*alloced = 2;
	*state = update_state(*input);
	*tokens = ft_calloc(*alloced, sizeof(t_token));
	if (!*tokens)
		return (FALSE);
	return (TRUE);
}
