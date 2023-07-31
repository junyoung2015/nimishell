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

t_token* realloc_tokens(t_token *tokens, t_size *cur_size, t_size new_size)
{
	t_size		i;
	t_size		cp_size;
	t_token*	new;

	if (new_size == 0)
		return (free_tokens(tokens, *cur_size));
	new = ft_calloc(new_size, sizeof(t_token));
	if (!new) 
		return (set_err_token(tokens, cur_size, MALLOC_ERR));
	cp_size = new_size;
	if (*cur_size < new_size)
		cp_size = *cur_size;
	ft_memcpy(new, tokens, cp_size * sizeof(t_token));
	i = -1;
	while (++i < cp_size)
	{
		new[i].val = ft_calloc(ft_strlen(tokens[i].val) + 1, 1);
		if (!new[i].val)
			return (set_err_token(tokens, cur_size, MALLOC_ERR));
		ft_memcpy(new[i].val, tokens[i].val, ft_strlen(tokens[i].val) + 1);
	}
	free_tokens(tokens, *cur_size);
	return (new);
}

t_token	*should_realloc(t_token *tokens, t_size *token_idx, t_size *alloced)
{
	t_token	*new;

	new = tokens;
    if (*alloced <= *token_idx + 1)
	{
        *alloced *= 2;
        new = realloc_tokens(tokens, token_idx, *alloced);
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

t_bool	is_meta_ch(char ch)
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

//A 1001
//B 0101
//(A ^ B)		1001 ^ 0101 = 1100;
//(A ^ B ^ A)	1100 ^ 1001 = 0101
//(A ^ B ^ B)	1100 ^ 0101 = 1001
t_bool	advance_to_next_token(char **input, t_cmp cmp)
{
	t_bool	(*tmp)(char);

	tmp = (t_cmp)((t_size)is_squote ^ (t_size)is_dquote);
	while (**input && !is_meta_ch(**input))
	{
		(*input)++;
		if (**input)
		{
			while (**input && !cmp(**input))
				(*input)++;
			if (cmp(**input))
				(*input)++;
			else
				return (FALSE);
		}
		else if (cmp(*(*input - 1)))
			return (FALSE);
		else
			return (TRUE);
		while (**input && !is_meta_ch(**input) && !is_quote(**input))
			(*input)++;
		if (!(**input) || is_meta_ch(**input))
			return (TRUE);
		else if (!cmp(**input))
			cmp =  (t_cmp)((t_size)tmp ^ (t_size)cmp);
	}
	return (TRUE);
}

t_token	*split_input(char *start, char **input, t_cmp cmp, t_token_type type)
{
	t_bool	quote;
	t_token	*new_token;
	t_cmp	tmp;

	quote = FALSE;
	tmp = (t_cmp)((t_size)is_squote ^ (t_size)is_dquote);
	if (is_quote((*input)[0]))
	{
		quote = TRUE;
		if (!advance_to_next_token(input, cmp))
			return (create_err_token(QUOTE_NOT_CLOSED));
	}
	else
	{
		(*input)++;
		while (**input && !cmp((*input)[0]) && !is_quote((*input)[0]))
			(*input)++;
		if (is_quote((*input)[0]))
		{
			cmp = (t_cmp)((t_size)tmp ^ (t_size)is_squote);
			if (is_squote((*input)[0]))
				cmp = (t_cmp)((t_size)tmp ^ (t_size)is_dquote);
			quote = TRUE;
			if (!advance_to_next_token(input, cmp))
				return (create_err_token(QUOTE_NOT_CLOSED));
		}
	}
	new_token = create_token(type, start, *input - start + quote);
	if (!new_token)
		return (0);
	return (new_token);
}

t_token_state update_state(char ch)
{
	if (is_squote(ch))
		return (SQUOTE);
	else if (is_dquote(ch))
		return (DQUOTE);
	else if (is_meta_ch(ch) || is_space(ch))
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

t_token *tokenize_input(char *input, t_size *num_tokens)
{
	t_size				alloced;
	t_size				token_idx;
	t_token				*tokens;
	t_token				*new_token;
	t_token_state		state;
	const t_cmp			cmp_func[] = { is_meta_ch, is_squote, is_dquote, is_space };

	state = update_state(*input);
	alloced = 2;
	token_idx = 0;
	tokens = ft_calloc(alloced, sizeof(t_token));
	if (!tokens)
		return (0);
	while (*input)
	{
		new_token = tokenize(&input, cmp_func[state], (int) state, &state);
		// new_token = tokenizers[state](&input, &state);
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
		tokens = should_realloc(tokens, &token_idx, &alloced);
		if (!tokens)
			return (0);
	}
	// TODO: malloc err?
	if (!tokens)
		return (0);
	*num_tokens = token_idx;
	if (!check_parenthesis(tokens, *num_tokens))
		tokens = set_err_token(tokens, num_tokens, PAREN_NOT_CLOSED);
	return (tokens);
}
