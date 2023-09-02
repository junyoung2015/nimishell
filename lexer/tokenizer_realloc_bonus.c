/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_realloc_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 11:34:41 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:57:09 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_token	*realloc_tokens(t_token *tokens, t_size *num_tokens, t_size new_size)
{
	t_size		i;
	t_size		cp_size;
	t_token		*new;

	if (new_size == 0)
		return (free_tokens(tokens, *num_tokens));
	new = ft_calloc(new_size, sizeof(t_token));
	if (!new)
		return (set_err_token(tokens, num_tokens, MALLOC_ERR));
	cp_size = new_size;
	if (*num_tokens < new_size)
		cp_size = *num_tokens;
	ft_memcpy(new, tokens, cp_size * sizeof(t_token));
	i = -1;
	while (++i < cp_size)
	{
		new[i].val = ft_calloc(ft_strlen(tokens[i].val) + 1, 1);
		if (!new[i].val)
			return (set_err_token(tokens, num_tokens, MALLOC_ERR));
		ft_memcpy(new[i].val, tokens[i].val, ft_strlen(tokens[i].val) + 1);
	}
	free_tokens(tokens, *num_tokens);
	return (new);
}

t_token	*should_realloc(t_token *tokens, t_size *num_tokens, t_size *alloced)
{
	t_token	*new;

	new = tokens;
	if (*alloced <= *num_tokens + 1)
	{
		*alloced = *alloced * 2;
		new = realloc_tokens(tokens, num_tokens, *alloced);
	}
	return (new);
}
