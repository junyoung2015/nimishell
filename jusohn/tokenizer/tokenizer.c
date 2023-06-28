/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:00:41 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/28 15:42:48 by jusohn           ###   ########.fr       */
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
		return (free_tokens(tokens, current_size));
	new_tokens = ft_calloc(new_size, sizeof(t_token));
	if (!new_tokens) 
	{
		write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
		write(STD_ERR, "\n", 1);
		return (free_tokens(tokens, current_size));
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
			return (free_tokens(tokens, i));
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
	if (ch == '|' || ch == '>' || ch == '<' || ch == '$' || ch == '\'' || \
		ch == '"' || ch == '(' || ch == ')' || ch == '&' || ch == '\t' || \
		ch == '\n' || ch == ' ')
		return (FT_TRUE);
	return (FT_FALSE);
}

t_token	*create_token(t_token_type type, const char *buffer, int buffer_length)
{
	t_token	*new_token;
	
	new_token = (t_token *) ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (0);
	new_token->type = type;
	// Allocate memory for the token value and copy the buffer content
	new_token->value = (char *) ft_calloc(buffer_length + 1, sizeof(char));
	if (!new_token->value)
	{
		free(new_token);
		return (0);
	}
	ft_strlcpy(new_token->value, buffer, buffer_length + 1);
	// new_token->value[buffer_length] = '\0';
	return (new_token);
}

t_token *tokenize(const char *input, t_size *num_tokens)
{
	t_size		    alloced;
	t_size		    token_idx;
	t_token		    *tokens;
	t_token         *new_token;
	t_size          subshell_depth = 0;
	t_token_type	type;
	char            *start;
	char			quote;

	alloced = 2;
	token_idx = 0;
	start = (char *) input;
	tokens = ft_calloc(alloced, sizeof(t_token));
	if (!tokens)
		return (0);
	while (*input)
	{
		if (is_meta_char(*input))
		{
			type = TOKEN_OPERATOR;
			if (*input == '\'' || *input == '"')
			{
				quote = *input;
				input++;
				while (*input && *input != quote)
					input++;
				// when closing quote is not found, TODO: free tokens
				if (!*input)
					return (0);
				type = TOKEN_DQ_STR;
				if (quote == '\'')
					type = TOKEN_SQ_STR;
			}
			new_token = create_token(type, start, input - start + 1);
			// TODO: free tokens
			if (!new_token)
				return (0);
		}
		else
		{
			while (*input && !is_meta_char(*input))
				input++;
			new_token = create_token(TOKEN_WORD, start, input - start);
			// TODO: free tokens
			if (!new_token)
				return (0);
			input--;
		}
		tokens[token_idx++] = *new_token;
		free(new_token);
		if (alloced <= token_idx + 1)
		{
			alloced *= 2;
			tokens = realloc_tokens(tokens, token_idx, alloced);
			if (!tokens)
				return (0);
		}
		input++;
		start = input;
	}
	tokens = realloc_tokens(tokens, alloced, token_idx);
	if (!tokens)
		return (0);
	*num_tokens = token_idx;
	return (tokens);
}

void print_tokens(t_token *tokens, t_size num_tokens)
{
	for (t_size i = 0; i < num_tokens; i++) {
		printf("token[%llu]: %d, %s\n", i, tokens[i].type, tokens[i].value);
	}
}
