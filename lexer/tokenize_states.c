
#include "minishell.h"

t_token*	tokenize_normal(char **input, t_token_state *state)
{
	t_token	*new_token;

	new_token = split_until(*input, input, is_meta_ch, TOKEN_WORD);
	if (!new_token)
		return (0);
	*state = update_state(**input);
    return (new_token);
}

t_token *tokenize_squote(char **input, t_token_state *state)
{
	t_token	*new_token;

	new_token = split_until(*input, input, is_squote, TOKEN_SQ_STR);
	if (!new_token)
		return (0);
	*state = update_state(**input);
    return (new_token);
}

t_token *tokenize_dquote(char **input, t_token_state *state)
{
	t_token	*new_token;

	new_token = split_until(*input, input, is_dquote, TOKEN_DQ_STR);
	if (!new_token)
		return (0);
	*state = update_state(**input);
    return (new_token);
}

t_token	*tokenize_meta(char **input, t_token_state *state)
{
	t_token	*new_token;

	if (is_space(**input))
		new_token = tokenize_whitespace(input, state);
	else
		new_token = tokenize_operator(input, state);
	if (!new_token)
		return (0);
	*state = update_state(**input);
	return (new_token);
}

t_token *tokenize_whitespace(char **input, t_token_state *state)
{
	t_token	*new_token;

	new_token = split_until(*input, input, is_not_space, TOKEN_WHITESPACE);
	if (!new_token)
		return (0);
	*state = update_state(**input);
    return (new_token);
}
