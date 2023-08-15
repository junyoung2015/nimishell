/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_err_handling.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:17:51 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 12:48:05 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_bool	check_parenthesis(t_token* tokens, t_size num_tokens)
{
	int		depth;
	t_size	i;

	i = 0;
	depth = 0;
	while (i < num_tokens)
	{
		if (tokens[i].type == TOKEN_L_PAREN)
			depth++;
		else if (tokens[i].type == TOKEN_R_PAREN)
			depth--;
		if (depth < 0)
			break ;
		i++;
	}
	return (depth == 0);
}
