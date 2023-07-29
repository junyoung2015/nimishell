#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

typedef struct s_token
{
	t_token_type type;
	char *val;
} t_token;

typedef t_token *(*t_tokenizer_fn)(char **, t_token_state *);

t_token	*free_tokens(t_token *tokens, t_size size);
t_token	*handle_malloc_err_in_tokenizer(t_token *tokens, t_size num_tokens);
t_token *realloc_tokens(t_token *tokens, t_size cur_size, t_size new_size);
t_token	*check_size(t_token *tokens, t_size token_idx, t_size *alloced);
t_bool	check_parenthesis(t_token* tokens, t_size num_tokens);
t_token	*unmatched_parenthesis(t_token *tokens, t_size *num_tokens);

t_bool			is_dmeta_ch(char ch);
t_bool			is_dmeta_str(char *input);
t_bool			is_escaped(char ch);
t_bool			is_meta_ch(char ch);
t_bool			is_squote(char ch);
t_bool			is_dquote(char ch);
t_bool			is_quote(char ch);
t_bool			is_space(char ch);
t_bool			is_not_space(char ch);
t_bool			is_env_var(char ch); // ?
t_token_type	get_operator_type(char ch);

t_token			*create_token(t_token_type type, const char *buffer, t_size buf_len);
t_token			*split_until(char *start, char **input, t_bool (*cmp)(char ch), t_token_type type);
t_bool			advance_to_next_token(char **input, t_bool (*cmp)(char ch));

t_token_state	update_state(char ch);
t_token			*tokenize_normal(char **input, t_token_state *state);
t_token			*tokenize_squote(char **input, t_token_state *state);
t_token			*tokenize_dquote(char **input, t_token_state *state);
t_token			*tokenize_operator(char **input, t_token_state *state);
t_token			*tokenize_meta(char **input, t_token_state *state);
t_token			*tokenize_whitespace(char **input, t_token_state *state);
t_token			*tokenizer(char *input, t_size *num_tokens);

#endif