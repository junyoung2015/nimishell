/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:00:41 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/30 13:58:49 by jusohn           ###   ########.fr       */
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

t_bool  is_meta_char(char ch)
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

// t_bool	is_env_var(char ch)
// {
// 	return (ch);
// }

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

t_token	*split_until(char *start, char **input, int (*cmp)(char ch), t_token_type type)
{
	t_token	*new_token;

	// if (type == TOKEN_DOLLAR_SIGN && **input)
	// {
	// 	(*input)++;
	// 	if (**input && **input == '(')
	// 	{

	// 	}
	// 	else if (**input && is_env_var(**input))
	// 	{

	// 	}
	// 	else 
	// 	{

	// 	}
	// }
	// else
	// {
		while (**input && !cmp(**input))
			(*input)++;
		new_token = create_token(type, start, *input - start);
		// TODO: free tokens
		// TODO: malloc err
		if (!new_token)
			return (0);
		(*input)--;
	// }
	return (new_token);
}

t_token *split_word(char *start, char **input)
{
	t_token	*new_token;

	while (**input && !is_meta_char(**input))
		(*input)++;
	new_token = create_token(TOKEN_WORD, start, *input - start);
	// TODO: free tokens
	// TODO: malloc err
	if (!new_token)
		return (0);
	(*input)--;
	return (new_token);
}

t_token *tokenize(char *input, t_size *num_tokens)
{
	t_size		    alloced;
	t_size		    token_idx;
	t_token		    *tokens;
	t_token         *new_token;
	t_token_type	type;
	char            *start;

	alloced = 2;
	token_idx = 0;
	start = (char *) input;
	tokens = ft_calloc(alloced, sizeof(t_token));
	// TODO: malloc err
	if (!tokens)
		return (0);
	while (*input)
	{
		if (is_meta_char(*input))
		{
			type = TOKEN_OPERATOR;
			if (is_dquote(*input))
			{
				new_token = split_until(start, &input, is_dquote, TOKEN_DQ_STR);
			}
			else if (is_squote(*input))
			{
				new_token = split_until(start, &input, is_squote, TOKEN_SQ_STR);
			}
			// else if (*input == '$')
			// {
			// 	new_token = split_until(start, &input, )
			// }
			else
				new_token = create_token(type, start, input - start + 1);
			// TODO: free tokens
			if (!new_token)
			// TODO: malloc err
				return (0);
		}
		else
		{
			new_token = split_until(start, &input, is_meta_char, TOKEN_WORD);
			// TODO: free tokens
			// TODO: malloc err
			if (!new_token)
				return (0);
			// input--;
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
		start = input;
	}
	tokens = realloc_tokens(tokens, alloced, token_idx);
	// TODO: malloc err
	if (!tokens)
		return (0);
	*num_tokens = token_idx;
	return (tokens);
}
