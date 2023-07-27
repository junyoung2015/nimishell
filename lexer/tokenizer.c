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

// TODO: re-name this funciton
t_token	*handle_malloc_err_in_tokenizer(t_token *tokens, t_size num_tokens)
{
	write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
	write(STD_ERR, "\n", 1);
	free_tokens(tokens, num_tokens);
	return (0);
}

t_token* realloc_tokens(t_token *tokens, t_size cur_size, t_size new_size)
{
	t_size		i;
	t_size		cp_size;
	t_token*	new;

	if (new_size == 0)
		return (free_tokens(tokens, cur_size));
	new = ft_calloc(new_size, sizeof(t_token));
	if (!new) 
		return (handle_malloc_err_in_tokenizer(tokens, cur_size));
	cp_size = new_size;
	if (cur_size < new_size)
		cp_size = cur_size;
	ft_memcpy(new, tokens, cp_size * sizeof(t_token));
	i = -1;
	while (++i < cp_size)
	{
		new[i].val = ft_calloc(ft_strlen(tokens[i].val) + 1, 1);
		if (!new[i].val)
			return (handle_malloc_err_in_tokenizer(tokens, cur_size));
		ft_memcpy(new[i].val, tokens[i].val, ft_strlen(tokens[i].val) + 1);
	}
	free_tokens(tokens, cur_size);
	return (new);
}

t_token	*check_size(t_token *tokens, t_size token_idx, t_size *alloced)
{
	t_token	*new;

    if (*alloced <= token_idx + 1)
	{
        *alloced *= 2;
        new = realloc_tokens(tokens, token_idx, *alloced);
    }
	else
		return (tokens);
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

t_token	*unmatched_parenthesis(t_token *tokens, t_size *num_tokens)
{
	t_token	*new_token;

	free_tokens(tokens, *num_tokens);
	tokens = ft_calloc(1, sizeof(t_token));
	if (!tokens)
		return (0);
	new_token = create_token(TOKEN_ERROR, PAREN_NOT_CLOSED, ft_strlen(PAREN_NOT_CLOSED));
	if (!new_token)
		return (0);
	*num_tokens = 0;
	tokens[(*num_tokens)++] = *new_token;
	free(new_token);
	return (tokens);
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

t_bool	is_escaped(char ch)
{
	return (ch == '\\');
}

t_bool	is_meta_ch(char ch)
{
	return (ch == '|' || ch == '>' || ch == '<' || ch == '(' \
		|| ch == ')' || ch == '&' || ch == '\t' || ch == '\n' || ch == ' ');
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

/**
 * @brief Check whether 'ch' is a valid character for an environment variable.
 * 
 * @param ch		character to check
 * @return t_bool	TRUE if 'ch' is a valid character for an environment variable,
 * 					FALSE otherwise.
 */
t_bool	is_env_var(char ch)
{
	return (ft_isalnum(ch) || ch == '_');
}

t_token_type	get_operator_type(char ch)
{
	t_token_type	type;

	if (ch == '|')
		type = TOKEN_PIPE;
	else if (ch == '<')
		type = TOKEN_REDIR_IN;
	else if (ch == '>')
		type = TOKEN_REDIR_OUT;
	else if (ch == '(')
		type = TOKEN_L_PAREN;
	else if (ch == ')')
		type = TOKEN_R_PAREN;
	else if (ch == '*')
		type = TOKEN_WILDCARD;
	else
		type = TOKEN_UNKNOWN;
	return (type);
}

t_token	*create_token(t_token_type type, const char *buffer, int buf_len)
{
	t_token	*new_token;
	
	new_token = (t_token *) ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (0);
	new_token->type = type;
	// Allocate memory for the token value and copy the buffer content
	new_token->val = (char *) ft_calloc(buf_len + 1, sizeof(char));
	if (!new_token->val)
	{
		free(new_token);
		return (0);
	}
	ft_strlcpy(new_token->val, buffer, buf_len + 1);
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
	if ((cmp == is_squote || cmp == is_dquote) && !cmp(**input))
	{
		// free(new_token->val);
		// free(new_token);
		if (!(**input) || (cmp != is_dquote && cmp != is_squote))
			(*input)--;
		return (create_token(TOKEN_ERROR, QUOTE_NOT_CLOSED, ft_strlen(QUOTE_NOT_CLOSED)));
	}
	if ((cmp == is_squote || cmp == is_dquote) && **input && cmp(**input) && !is_meta_ch(**input))
	{
 		(*input)++;
		while (**input && !cmp(**input) && !is_meta_ch(**input))
			(*input)++;
		(*input)--;
	}
	new_token = create_token(type, start, *input - start + is_quote);
	// TODO: free tokens, malloc err?
	if (!new_token)
		return (0);
	if (!(**input) || (cmp != is_dquote && cmp != is_squote))
		(*input)--;
	return (new_token);
}

t_token_state update_state(char ch)
{
	if (ch == '\'')
		return (SQUOTE);
	else if (ch == '"')
		return (DQUOTE);
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
	*state = update_state(*(*input + 1));
    return (new_token);
}

t_token *tokenize_dquote(char **input, t_token_state *state)
{
	t_token	*new_token;
	(void)	state;

	new_token = split_until(*input, input, is_dquote, TOKEN_DQ_STR);
	if (!new_token)
		return (0);
	*state = update_state(*(*input + 1));
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

t_token *tokenize_whitespace(char **input, t_token_state *state)
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
		}
		else if (tokens[token_idx - 1].type == TOKEN_WHITESPACE)
			free(tokens[token_idx-- - 1].val);
		tokens = check_size(tokens, token_idx, &alloced);
		if (!tokens)
			return (0);
		input++;
	}
	// TODO: malloc err?
	if (!tokens)
		return (0);
	*num_tokens = token_idx;
	if (!check_parenthesis(tokens, *num_tokens))
		tokens = unmatched_parenthesis(tokens, num_tokens);
	return (tokens);
}
