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

t_token* realloc_tokens(t_token *tokens, t_size *num_tokens, t_size new_size)
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

t_bool	is_squote(char ch)
{
	return (ch == '\'');
}

t_bool	is_dquote(char ch)
{
	return (ch == '"');
}

t_bool	is_quote(char ch)
{
	return (is_squote(ch) || is_dquote(ch));
}

t_bool	is_space(char ch)
{
	return (ch == ' ' || ch == '\t' || ch == '\f' || ch == '\n' || \
		ch == '\v' || ch == '\r');
}

t_bool	is_not_space(char ch)
{
	return (!is_space(ch));
}

t_bool	is_meta(char ch)
{
	return (ch == '|' || ch == '>' || ch == '<' || ch == '(' \
		|| ch == ')' || ch == '&' || is_space(ch));
}

t_bool	is_dmeta_ch(char ch)
{
	return (ch == '|' || ch == '>' || ch == '<' || ch == '&');
}

/**
 * @brief Check whether 'input' matches double-character operators.
 * 
 * @param input 
 * @return t_token_type
 * 		enum, but also works as TRUE or FALSE, since TOKEN_WORD = 0,
 * 		and other enums are above 0.
 */
t_bool	is_dmeta_str(char *input)
{
	if (!(is_dmeta_ch(0[input]) && is_dmeta_ch(1[input])))
		return (TOKEN_WORD);
	if (!ft_strncmp(input, "<<", 2))
		return (TOKEN_HEREDOC);
	else if (!ft_strncmp(input, ">>", 2))
		return (TOKEN_APPEND);
	else if (!ft_strncmp(input, "||", 2))
		return (TOKEN_OR);
	else if (!ft_strncmp(input, "&&", 2))
		return (TOKEN_AND);
	return (TOKEN_WORD);
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
			free(tokens[(*num_tokens)-- - 1].val);
		tokens = should_realloc(tokens, num_tokens, &alloced);
		if (!tokens)
			return (set_err_token(tokens, num_tokens, MALLOC_ERR));
	}
	if (!check_parenthesis(tokens, *num_tokens))
		tokens = set_err_token(tokens, num_tokens, PAREN_NOT_CLOSED);
	return (tokens);
}
