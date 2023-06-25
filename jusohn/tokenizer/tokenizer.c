/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 14:00:41 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/25 21:36:50 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token* realloc_tokens(t_token* tokens, t_size current_size, t_size new_size) {
    t_token* new_tokens;
    t_size  copy_size;
    t_size  value_length;
    
    if (new_size == 0)
    {
        for (size_t i = 0; i < current_size; i++)
        {
            free(tokens[i].value);
            tokens[i].value = 0;
        }
        free(tokens);
        return (0);
    }
    new_tokens = ft_calloc(new_size, sizeof(t_token));
    if (!new_tokens) 
    {
        write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
        write(STD_ERR, "\n", 1);
        // fprintf(stderr, "Memory allocation error\n");
        return (0);
    }
    copy_size = current_size < new_size ? current_size : new_size;
    ft_memcpy(new_tokens, tokens, copy_size * sizeof(t_token));
    // Copy 'value' strings
    for (size_t i = 0; i < copy_size; i++)
    {
        value_length = strlen(tokens[i].value) + 1;
        new_tokens[i].value = (char *) malloc(value_length * sizeof(char));
        if (!new_tokens[i].value)
        {
            write(STD_ERR, MALLOC_ERR, ft_strlen(MALLOC_ERR));
            write(STD_ERR, "\n", 1);
            // fprintf(stderr, "Memory allocation error\n");
            // Free already allocated strings and memory
            for (size_t j = 0; j < i; j++) {
                free(new_tokens[j].value);
            }
            free(new_tokens);
            return (0);
        }
        ft_memcpy(new_tokens[i].value, tokens[i].value, value_length * sizeof(char));
        free(tokens[i].value);
    }
    // Free original 'value' strings and memory
    // for (size_t i = 0; i < current_size; i++)
    //     free(tokens[i].value);
    free(tokens);
	tokens = 0;
    return (new_tokens);
}

t_bool  is_special_char(char ch)
{
    if (ch == '|' || ch == '>' || ch == '<' || ch == '$' || ch == '\'' || ch == '"' \
        || ch == '(' || ch == ')' || ch == '&')
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
    char            *start;
    char            *var_start;
    t_token         *new_token;
    t_bool          in_squote;
    t_bool          in_dquote;
    t_bool          *in_quote;
    t_token_type    quote_type;
    // Add this variable for tracking subshell depth
    int subshell_depth = 0;
    // Define a dynamic array to hold tokens
    // Define variables to keep track of the current token start and length
	alloced = 2;
    token_idx = 0;
    start = (char *) input;
    in_squote = FT_FALSE;
    in_dquote = FT_FALSE;
	tokens = ft_calloc(alloced, sizeof(t_token));
	if (!tokens)
		return (0);
    // Loop through each character in the input
    while (*input)
    {
        if (*input == '$' && !in_squote)
        {
            var_start = (char *)input; // Include the dollar sign
            while (*(input + 1) && (ft_isalnum(*(input + 1)) || *(input + 1) == '_'))
                input++;
            new_token = create_token(TOKEN_ENV_VAR, var_start, input - var_start + 1); // Increase buffer length
            if (!new_token)
                return (0);
            tokens[token_idx++] = *new_token;
            free(new_token);
            start = (char *)input + 1;
        }
        else if ((*input == '\'' && !in_dquote) || (*input == '"' && !in_squote))
        {
            char quote_char = *input; // Store the quote character (either ' or ")
            start = (char *)input; // Include the quote in the token
            input++; // Move to the next character

            // Find the matching closing quote
            while (*input && !(*input == quote_char && !subshell_depth))
            {
                // Keep track of subshell depth
                if (*input == '$' && *(input + 1) == '(')
                    subshell_depth++;
                else if (*input == ')' && subshell_depth > 0)
                    subshell_depth--;

                input++;
            }

            // Create a token for the entire content inside quotes including quotes
            new_token = create_token(TOKEN_WORD, start, input - start + 1);
            if (!new_token)
                return (0);
            tokens[token_idx++] = *new_token;
            free(new_token);
            start = (char *)input + 1; // update start to point after the closing quote
        }
        // else if ((*input == '\'' && !in_dquote) || (*input == '"' && !in_squote))
        // {
        //     if (*input == '\'')
        //         in_quote = &in_squote;
        //     else
        //         in_quote = &in_dquote;
        //     quote_type = TOKEN_S_QUOTE;
        //     if (in_quote == &in_dquote)
        //         quote_type = TOKEN_D_QUOTE;
        //     *in_quote = !*in_quote;
        //     if (*in_quote)
        //     {
        //         // opening quote
        //         new_token = create_token(quote_type, input, 1); // quote as separate token
        //         if (!new_token)
        //             return (0);
        //         tokens[token_idx++] = *new_token;
        //         free(new_token);
        //         start = (char *)input + 1; // update start to point after the quote
        //     }
        //     else
        //     {
        //         // closing quote, process the contents
        //         if (input > start)
        //         {
        //             new_token = create_token(TOKEN_WORD, start, input - start); // contents
        //             if (!new_token)
        //                 return (0);
        //             tokens[token_idx++] = *new_token;
        //             free(new_token);
        //         }
        //         new_token = create_token(quote_type, input, 1); // quote as separate token
        //         if (!new_token)
        //             return (0);
        //         tokens[token_idx++] = *new_token;
        //         free(new_token);
        //         start = (char *)input + 1;
        //     }
        // }
        else if (!in_squote && !in_dquote && (is_special_char(*input) || *input == ' '))
        {
            // End the current token (if there is one)
            // Add the special character or delimiter as a separate token
            // Update the current token start and length
            if (input > start)
            {
                // end of a token
                new_token = create_token(TOKEN_UNKNOWN, start, input - start);
                // free the saved tokens?
                if (!new_token)
                    return (0);
                tokens[token_idx++] = *new_token;
                free(new_token);
            }
            if (is_special_char(*input))
            {
                new_token = create_token(TOKEN_UNKNOWN, input, 1);
                // free the saved tokens?
                if (!new_token)
                    return (0);
                tokens[token_idx++] = *new_token;
                free(new_token);
            }
            start = (char *)input + 1;
        }
            if (alloced <= token_idx + 1)
            {
                alloced *= 2;
                tokens = realloc_tokens(tokens, token_idx, alloced);
                if (!tokens)
                    return (0);
            }
        input++;
    }
    if (start < input)
    {
        new_token = create_token(TOKEN_UNKNOWN, start, input - start);
        // free the saved tokens?
        if (!new_token)
            return (0);
        tokens[token_idx++] = *new_token;
        free(new_token);
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
