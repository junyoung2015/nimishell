/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 11:45:10 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 13:06:53 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	move_until_cmp(char **input, t_cmp cmp)
{
	while (**input && !is_meta(**input))
	{
		(*input)++;
		if (**input)
		{
			while (**input && !cmp(**input))
				(*input)++;
			if (!cmp(**input))
				return (FALSE);
			(*input)++;
		}
		else if (cmp(*(*input - 1)))
			return (FALSE);
		else
			return (TRUE);
		while (**input && !is_meta(**input) && !is_quote(**input))
			(*input)++;
		if (!**input || is_meta(**input))
			return (TRUE);
		cmp = get_cmp_fn(**input);
	}
	return (TRUE);
}

t_token	*split_input(char *start, char **input, t_cmp cmp, t_type type)
{
	t_bool	quote;
	t_token	*new_token;

	quote = is_quote((*input)[0]);
	if (quote)
	{
		if (!move_until_cmp(input, cmp))
			return (create_err_token(QUOTE_NOT_CLOSED));
	}
	else
	{
		(*input)++;
		while (**input && !cmp((*input)[0]) && !is_quote((*input)[0]))
			(*input)++;
		if (**input && !cmp((*input)[0]) && is_quote((*input)[0]))
		{
			cmp = get_cmp_fn((*input)[0]);
			quote = TRUE;
			if (!move_until_cmp(input, cmp))
				return (create_err_token(QUOTE_NOT_CLOSED));
		}
	}
	quote = is_quote((*input)[0]);
	new_token = create_token(type, start, *input - start + quote);
	return (new_token);
}
