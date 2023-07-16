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

int	free_tokens(t_token *tokens, t_size size)
{
	t_size	i;

	i = 0;
	if (size != 0)
		while (i < size)
			free(tokens[i++].value);
	free(tokens);
	return (0);
}

t_token* realloc_tokens(t_token* tokens, t_size cur_size, t_size new_size)
{
	t_size		len;
	t_size		cp_size;
	t_token*	new_tokens;
	
	if (new_size == 0)
	{
		free_tokens(tokens, cur_size);
		return (0);
	}
	new_tokens = ft_calloc(new_size, sizeof(t_token));
	if (!new_tokens) 
	{
		write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
		write(STD_ERR, "\n", 1);
		free_tokens(tokens, cur_size);
		return (0);
	}
	cp_size = cur_size < new_size ? cur_size : new_size;
	ft_memcpy(new_tokens, tokens, cp_size * sizeof(t_token));
	for (size_t i = 0; i < cp_size; i++)
	{
		len = ft_strlen(tokens[i].value) + 1;
		new_tokens[i].value = (char *) ft_calloc(len, sizeof(char));
		if (!new_tokens[i].value)
		{
			write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
			write(STD_ERR, "\n", 1);
			free_tokens(tokens, i);
			return (0);
		}
		ft_memcpy(new_tokens[i].value, tokens[i].value, len * sizeof(char));
		free(tokens[i].value);
	}
	free(tokens);
	tokens = 0;
	return (new_tokens);
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

t_bool	is_meta_ch(char ch)
{
	return (ch == '|' || ch == '>' || ch == '<' || ch == '$' || ch == '\'' || \
		ch == '"' || ch == '(' || ch == ')' || ch == '&' || ch == '\t' || \
		ch == '\n' || ch == ' ');
}

t_bool	is_dquote(char ch)
{
	return (ch == '"');
}

t_bool	is_squote(char ch)
{
	return (ch == '\'');
}

t_bool	is_space(char ch)
{
	return (ch == ' ' || ch == '\t' || ch == '\f' || ch == '\n' || \
		ch == '\v' || ch == '\r');
}

t_token	*create_token(t_token_type type, const char *buffer, int buf_len)
{
	t_token	*new_token;
	
	new_token = (t_token *) ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (0);
	new_token->type = type;
	// Allocate memory for the token value and copy the buffer content
	new_token->value = (char *) ft_calloc(buf_len + 1, sizeof(char));
	if (!new_token->value)
	{
		free(new_token);
		return (0);
	}
	ft_strlcpy(new_token->value, buffer, buf_len + 1);
	return (new_token);
}

t_token	*split_until(char *start, char **input, t_bool (*cmp)(char ch), t_token_type type)
{
	t_bool	is_quote;
	t_token	*new_token;

	is_quote = FALSE;
	if (cmp == is_dquote || cmp == is_squote)
		is_quote = TRUE;
	(*input)++;
	while (**input && !cmp(**input))
		(*input)++;
	new_token = create_token(type, start, *input - start + is_quote);
	// TODO: free tokens, malloc err
	if (!new_token)
		return (0);
	if ((cmp == is_squote || cmp == is_dquote) && !cmp(**input))
	{
		free(new_token->value);
		free(new_token);
		if (!(**input) || (cmp != is_dquote && cmp != is_squote))
			(*input)--;
		return (create_token(TOKEN_ERROR, QUOTE_NOT_CLOSED, ft_strlen(QUOTE_NOT_CLOSED)));
	}
	if (!(**input) || (cmp != is_dquote && cmp != is_squote))
		(*input)--;
	return (new_token);
}

t_token_state update_state(char ch)
{
	if (is_meta_ch(ch))
		return (META_CH);
	else if (ch == '\0')
		return (END);
	return (NORMAL);
}

t_token*	tokenize_normal(char **input, t_token_state *state)
{
    char	*start;
	t_token	*new_token;

	start = *input;
	new_token = split_until(start, input, is_meta_ch, TOKEN_WORD);
	if (!new_token)
		return (0);
	*state = update_state(*(*input + 1));
    return (new_token);
}

t_token *tokenize_squote(char **input, t_token_state *state)
{
	char	*start;
	t_token	*new_token;
	(void)	state;

	start = *input;
	new_token = split_until(start, input, is_squote, TOKEN_SQ_STR);
	if (!new_token)
		return (0);
    return (new_token);
}

t_token *tokenize_dquote(char **input, t_token_state *state)
{
	t_token	*new_token;
	(void)	state;

	new_token = split_until(*input, input, is_dquote, TOKEN_DQ_STR);
	if (!new_token)
		return (0);
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
		new_token = create_token(TOKEN_OPERATOR, *input, 1);
	*state = update_state(*(*input + 1));
	return (new_token);
}

t_token	*tokenize_meta(char **input, t_token_state *state)
{
    char	*start;
	t_token	*new_token;

	start = *input;
	if (is_squote(*start))
		new_token = tokenize_squote(input, state);
	else if (is_dquote(*start))
		new_token = tokenize_dquote(input, state);
	else if (is_space(*start))
		new_token = tokenize_whitespace(input, state);
	else
		new_token = tokenize_operator(input, state);
	*state = update_state(*(*input + 1));
	return (new_token);
}

t_token* tokenize_whitespace(char **input, t_token_state *state)
{  
	char	*start;
	t_token	*new_token;

	start = *input;
    while (is_space(**input))
    {
		(*input)++;
	}
	new_token = create_token(TOKEN_WHITESPACE, start, *input - start);
	*state = update_state(**input);
	(*input)--;
    return (new_token);
}

t_token *tokenize_cmd(char *input, t_size *num_tokens)
{
	t_size				alloced;
	t_size				token_idx;
	t_token				*tokens;
	t_token				*new_token;
	t_token_state		state;
	const tokenizer_fn	tokenizers[] = {
		tokenize_normal,
		tokenize_squote,
		tokenize_dquote,
		tokenize_meta,
		tokenize_whitespace,
	};

	state = update_state(*input);
	alloced = 2;
	token_idx = 0;
	tokens = ft_calloc(alloced, sizeof(t_token));
	if (!tokens)
		return (0);
	while (*input)
	{
		new_token = tokenizers[state](&input, &state);
		if (!new_token)
		{
			// handle malloc err
			// free tokens here...? or main?
			return (0);
		}
		tokens[token_idx++] = *new_token;
		free(new_token);
		if (tokens[token_idx - 1].type == TOKEN_ERROR)
		{
			// handle syntax error, maybe send appropriate error message to exit_err or to main()
			break ;
			// return (tokens);
		}
		else if (tokens[token_idx - 1].type == TOKEN_WHITESPACE)
		{
			token_idx--;
		}
		else if (alloced <= token_idx + 1)
		{
			alloced *= 2;
			tokens = realloc_tokens(tokens, token_idx, alloced);
			// TODO: malloc err
			if (!tokens)
				return (0);
		}
		input++;
	}
	// TODO: malloc err
	if (!tokens)
		return (0);
	*num_tokens = token_idx;
	return (tokens);
}
