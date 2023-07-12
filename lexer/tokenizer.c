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

typedef t_token* (*tokenizer_fn)(char **, t_token_state *);
t_token *tokenize_normal(char **input, t_token_state *state);
// t_token *tokenize_esc(char **input, t_token_state *state);
t_token *tokenize_squote(char **input, t_token_state *state);
t_token *tokenize_dquote(char **input, t_token_state *state);
// t_token *tokenize_subshell(char **input, t_token_state *state);
t_token *tokenize_meta(char **input, t_token_state *state);
t_token *tokenize_whitespace(char **input, t_token_state *state);
// t_token *tokenize_operator(char **input, t_token_state *state);

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

t_token* realloc_tokens(t_token* tokens, t_size current_size, t_size new_size)
{
	t_token* new_tokens;
	t_size  copy_size;
	t_size  value_length;
	
	if (new_size == 0)
	{
		free_tokens(tokens, current_size);
		return (0);
	}
	new_tokens = ft_calloc(new_size, sizeof(t_token));
	if (!new_tokens) 
	{
		write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
		write(STD_ERR, "\n", 1);
		free_tokens(tokens, current_size);
		return (0);
	}
	copy_size = current_size < new_size ? current_size : new_size;
	ft_memcpy(new_tokens, tokens, copy_size * sizeof(t_token));
	for (size_t i = 0; i < copy_size; i++)
	{
		value_length = ft_strlen(tokens[i].value) + 1;
		new_tokens[i].value = (char *) ft_calloc(value_length, sizeof(char));
		if (!new_tokens[i].value)
		{
			write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
			write(STD_ERR, "\n", 1);
			free_tokens(tokens, i);
			return (0);
		}
		ft_memcpy(new_tokens[i].value, tokens[i].value, value_length * sizeof(char));
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

t_bool	is_dmeta_str(char *str)
{
	if (!(is_dmeta_ch(0[str]) && is_dmeta_ch(1[str])))
		return (FALSE);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (TRUE);
	else if (ft_strncmp(str, ">>", 2) == 0)
		return (TRUE);
	else if (ft_strncmp(str, "||", 2) == 0)
		return (TRUE);
	else if (ft_strncmp(str, "&&", 2) == 0)
		return (TRUE);
	return (FALSE);
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

t_bool	is_subshell_closed(char *str)
{
	t_size	cnt;

	cnt = 0;
	if (*str == '$')
		str++;
	if (*(str + 1) == '(')
	{
		while (*str)
		{
			if (*str == '(')
				cnt++;
			else if (*str == ')')
				cnt--;
			str++;
		}
	}
	if (cnt == 0)
		return (TRUE);
	return (FALSE);
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
	// new_token->value[buf_len] = '\0';
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
	if (!(**input) || (cmp != is_dquote && cmp != is_squote))
		(*input)--;
	return (new_token);
}

t_token *tokenize(char *input, t_size *num_tokens)
{
	t_size			alloced;
	t_size			token_idx;
	t_token			*tokens;
	t_token			*new_token;
	t_token_type	type;
	char			*start;

	alloced = 2;
	token_idx = 0;
	start = input;
	tokens = ft_calloc(alloced, sizeof(t_token));
	// TODO: malloc err
	if (!tokens)
		return (0);
	while (*input)
	{
		new_token = 0;
		if (is_meta_ch(*input))
		{
			if (*input != ' ')
			{
				type = TOKEN_OPERATOR;
				if (is_dquote(*input))
					new_token = split_until(start, &input, is_dquote, TOKEN_DQ_STR);
				else if (is_squote(*input))
					new_token = split_until(start, &input, is_squote, TOKEN_SQ_STR);
				else if (is_dmeta_str(input))
					new_token = create_token(type, start, (input++) - start + 2);
				// else if (is_env_var(input) || is_subshell(input))
				else
					new_token = create_token(type, start, input - start + 1);
				// else if (*input == '$')
				// {
				// 	new_token = split_until(start, &input, )
				// }
				// TODO: free tokens, malloc err
				if (!new_token)
					return (0);
			}
		}
		else
		{
			new_token = split_until(start, &input, is_meta_ch, TOKEN_WORD);
			// TODO: free tokens, malloc err
			if (!new_token)
				return (0);
		}
		if (new_token)
			tokens[token_idx++] = *new_token;
		free(new_token);
		if (alloced <= token_idx + 1)
		{
			alloced *= 2;
			tokens = realloc_tokens(tokens, token_idx, alloced);
			// TODO: malloc err
			if (!tokens)
				return (0);
		}
		input++;
		start = input;
	}
	tokens = realloc_tokens(tokens, alloced, token_idx);
	// TODO: malloc err
	if (!tokens)
		return (0);
	*num_tokens = token_idx;
	return (tokens);
}

t_token*	tokenize_normal(char **input, t_token_state *state)
{
    char	*start;
	t_token	*new_token;

	start = *input;
	new_token = split_until(start, &start, is_meta_ch, TOKEN_WORD);
	if (!new_token)
		return (0);
    if (*start == '\0')
        *state = END;
    else if (is_space(*start))
    {
		tokenize_whitespace(input, state);
		// *state = NORMAL;
	}
    else
        *state = META_CH;
    return (new_token);
}


t_token *tokenize_squote(char **input, t_token_state *state)
{
	char	*start;
	t_token	*new_token;

	start = *input;
    // increment str to skip initial quote
    // (*str)++;
    // find end of string or next single quote
    while (**input != '\0' && **input != '\'')
        (*input)++;
    // new_token = create_token(TOKEN_SQ_STR, start + 1, *input - start - 1);
    new_token = create_token(TOKEN_SQ_STR, start, *input - start + 1);
	if (!new_token)
		return (0);
    if (**input == '\0')
        *state = END;
    else
        *state = NORMAL;
    return (new_token);
}

t_token *tokenize_dquote(char **input, t_token_state *state)
{
	char	*start;
	t_token	*new_token;

	start = *input;
    // increment str to skip initial quote
    // (*str)++;
    // find end of string or next single quote
    while (**input != '\0' && **input != '\'')
        (*input)++;
    // new_token = create_token(TOKEN_DQ_STR, start + 1, *input - start - 1);
    new_token = create_token(TOKEN_DQ_STR, start, *input - start + 1);
	if (!new_token)
		return (0);
    if (**input == '\0')
        *state = END;
    else
        *state = NORMAL;
    return (new_token);
}

t_token_type	get_dmeta_type(char *str)
{
	t_token_type	type;

	if (ft_strncmp(str, "<<", 2) == 0)
		type = TOKEN_HEREDOC;
	else if (ft_strncmp(str, ">>", 2) == 0)
		type = TOKEN_APPEND;
	else if (ft_strncmp(str, "||", 2) == 0)
		type = TOKEN_OR;
	else if (ft_strncmp(str, "&&", 2) == 0)
		type = TOKEN_AND;
	else
		type = TOKEN_WORD;
	return (type);
}

t_token* tokenize_meta(char **str, t_token_state *state)
{
    char	*start;
	t_token	*new_token;

	start = *str;
	if (is_dquote(*start))
		new_token = tokenize_squote(str, state);
	else if (is_squote(*start))
		new_token = tokenize_dquote(str, state);
	else if (is_dmeta_str(start))
		new_token = create_token(get_dmeta_type(start), start, 2);
	else
		new_token = create_token(TOKEN_OPERATOR, start, 1);
	*state = NORMAL;
	return (new_token);
}

t_token* tokenize_whitespace(char **str, t_token_state *state)
{
    while (is_space(**str))
        (*str)++;
    if (**str == '\0')
        *state = END;
    else
        *state = NORMAL;
    return (0);
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
	};

	state = NORMAL;
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
			// handle syntax error
			// free tokens here...? or main?
			return (0);
		}
		tokens[token_idx++] = *new_token;
		free(new_token);
		if (alloced <= token_idx + 1)
		{
			alloced *= 2;
			tokens = realloc_tokens(tokens, token_idx, alloced);
			// TODO: malloc err
			if (!tokens)
				return (0);
		}
		input++;
	}
	tokens = realloc_tokens(tokens, alloced, token_idx);
	// TODO: malloc err
	if (!tokens)
		return (0);
	*num_tokens = token_idx;
	return (tokens);
}
