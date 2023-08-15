/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 11:22:59 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 11:34:28 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token_type type, const char *buffer, t_size buf_len)
{
	t_token	*new_token;
	
	new_token = (t_token *) ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (0);
	new_token->type = type;
	new_token->val = (char *) ft_calloc(buf_len + 1, sizeof(char));
	if (!new_token->val)
	{
		free(new_token);
		return (0);
	}
	ft_strlcpy(new_token->val, buffer, buf_len + 1);
	return (new_token);
}

t_token	*create_err_token(char *msg)
{
	t_token	*new_token;

	new_token = create_token(TOKEN_ERROR, msg, ft_strlen(msg));
	if (!new_token)
		return (0);
	return (new_token);
}

t_token	*set_err_token(t_token *tokens, t_size *num_tokens, char *msg)
{
	t_token	*new_token;

	free_tokens(tokens, *num_tokens);
	tokens = ft_calloc(1, sizeof(t_token));
	if (!tokens)
		return (0);
	new_token = create_err_token(msg);
	if (!new_token)
		return (0);
	*num_tokens = 0;
	tokens[(*num_tokens)++] = *new_token;
	free(new_token);
	return (tokens);
}

t_token	*free_tokens(t_token *tokens, t_size size)
{
	t_size	i;

	i = 0;
	if (size != 0)
		while (i < size)
			free(tokens[i++].val);
	free(tokens);
	return (0);
}
