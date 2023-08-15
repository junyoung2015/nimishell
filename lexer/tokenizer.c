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

t_token_type	get_operator_type(char ch)
{
	if (ch == '|')
		return (TOKEN_PIPE);
	else if (ch == '<')
		return (TOKEN_REDIR_IN);
	else if (ch == '>')
		return (TOKEN_REDIR_OUT);
	else if (ch == '(')
		return (TOKEN_L_PAREN);
	else if (ch == ')')
		return (TOKEN_R_PAREN);
	else if (ch == '*')
		return (TOKEN_WILDCARD);
	return (TOKEN_UNKNOWN);
}

/**
 * @brief Return corresponding compare function 'cmp' for 'ch'.
 * 
 * @param ch		cmp function
 * @return t_cmp	opposite cmp function
 */
t_cmp	get_cmp_fn(char ch)
{
	if (is_squote(ch))
		return (is_squote);
	return (is_dquote);
}

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

t_token	*split_input(char *start, char **input, t_cmp cmp, t_token_type type)
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

t_token_state update_state(char ch)
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

t_token	*tokenize(char **input, t_cmp cmp, t_token_type type, t_token_state *state)
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

t_token	*tokenize_operator(char **input, t_token_state *state)
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

t_token	*tokenize_meta(char **input, t_token_state *state)
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

t_bool	init_tokenizer(char *input, t_token **tokens, t_size *alloced,  t_token_state *state)
{
	*alloced = 2;
	*state = update_state(*input);
	*tokens = ft_calloc(*alloced, sizeof(t_token));
	if (!*tokens)
		return (FALSE);
	return (TRUE);
}

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
