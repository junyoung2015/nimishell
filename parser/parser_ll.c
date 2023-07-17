/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 19:05:20 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/17 19:05:20 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


typedef t_node* (*parser_fn)(t_token **, t_parse_state *);
t_node* parse_nonspace(t_token **tokens, t_parse_state *parse_state);
t_node* parse_word(t_token **tokens, t_parse_state *parse_state);
t_node* parse_env_var(t_token **tokens, t_parse_state *parse_state);
t_node* parse_word_list(t_token **tokens, t_parse_state *parse_state);
t_node* parse_word_list_tail(t_token **tokens, t_parse_state *parse_state);
t_node* parse_assignment_word(t_token **tokens, t_parse_state *parse_state);
t_node* parse_redir(t_token **tokens, t_parse_state *parse_state);
t_node* parse_simple_cmd_element(t_token **tokens, t_parse_state *parse_state);
t_node* parse_redir_list(t_token **tokens, t_parse_state *parse_state);
t_node* parse_redir_list_tail(t_token **tokens, t_parse_state *parse_state);
t_node* parse_simple_cmd(t_token **tokens, t_parse_state *parse_state);
t_node* parse_simple_cmd_tail(t_token **tokens, t_parse_state *parse_state);
t_node* parse_command(t_token **tokens, t_parse_state *parse_state);
t_node* parse_subshell(t_token **tokens, t_parse_state *parse_state);
t_node* parse_list(t_token **tokens, t_parse_state *parse_state);
t_node* parse_list_tail(t_token **tokens, t_parse_state *parse_state);
t_node* parse_pipeline(t_token **tokens, t_parse_state *parse_state);
t_node* parse_pipeline_tail(t_token **tokens, t_parse_state *parse_state);

const parser_fn parsers[PARSE_STATES_CNT] = {
	parse_nonspace,
	parse_word,
	parse_env_var,
	parse_word_list,
	parse_word_list_tail,
	parse_assignment_word,
	parse_redir,
	parse_simple_cmd_element,
	parse_redir_list,
	parse_redir_list_tail,
	parse_simple_cmd,
	parse_simple_cmd_tail,
	parse_command,
	parse_subshell,
	parse_list,
	parse_list_tail,
	parse_pipeline,
	parse_pipeline_tail
};

void	syntax_err(t_token *token)
{
	write(STD_ERR, "Error!\n", 8);
}

// t_token *peek(t_token *token) {
// }

// t_token *consume(t_token *parser) {
// }

// t_bool consume_if(t_token *token, t_token_type type) {
// }

// t_bool expect(t_token *token, t_token_type type) {
//     if (peek(token)->type == type) {
//         consume(token);
//         return TRUE;
//     }
//     syntax_err(token);
//     return (FALSE);
// }

// t_node	*parse_command(t_token *tokens, t_size *idx, t_size num_tokens, t_parse_state *state)
// {
// 	t_node	*new_node;
// 	t_node	*pipe_node;

// 	new_node = parse_simple_cmd(tokens, idx, num_tokens, state);
// 	if (*idx < num_tokens && (*tokens)[*idx].type == TOKEN_PIPE )
// 	{
// 		pipe_node = create_node(AST_PIPE);
// 		(*idx)++;
// 		pipe_node->left = new_node;
// 		pipe_node->right = parse_command(token, idx, num_tokens, state);
// 		return (pipe_node);
// 	}
// 	return (new_node);
// }

// t_node	*parse_tokens_ll(t_token *tokens, t_size num_tokens)
// {
// 	t_size	idx;
// 	t_node	*root;
// 	t_parse_state state;

// 	state = CMD;
// 	root = 0;
// 	idx = 0;
// 	root = parse_command(&tokens, &idx, num_tokens, &state);
// 	return (root);
// }

t_node *parse(t_token *tokens, t_size num_tokens)
{
    t_node *root;
    t_parse_state state;
	t_token *token;
	t_node *node;
	
	state = CMD;

    root = 0;
    for (t_size i = 0; i < num_tokens; i++)
	{
        token = &tokens[i];
        // parser_fn parser = parsers[state];
        node = parser(&token, &state);
        if (!node)
            return (0);
        // For now just setting the root node here
        root = node;
        state = update_state(state, token->type);
    }

    return root;
}