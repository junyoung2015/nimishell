/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 19:05:20 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/18 22:28:48 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// typedef t_node* (*parse_fn)(t_parser*, t_parse_state*);

// t_node* parse_nonspace(t_parser *parser, t_parse_state *parse_state);
t_node* parse_word(t_parser *parser, t_parse_state *parse_state);
t_node* parse_env_var(t_parser *parser, t_parse_state *parse_state);
t_node* parse_word_list(t_parser *parser, t_parse_state *parse_state);
t_node* parse_word_list_tail(t_parser *parser, t_parse_state *parse_state);
t_node* parse_assignment_word(t_parser *parser, t_parse_state *parse_state);
t_node* parse_redir(t_parser *parser, t_parse_state *parse_state);
t_node* parse_simple_cmd_element(t_parser *parser, t_parse_state *parse_state);
t_node* parse_redir_list(t_parser *parser, t_parse_state *parse_state);
t_node* parse_redir_list_tail(t_parser *parser, t_parse_state *parse_state);
t_node* parse_simple_cmd(t_parser *parser, t_parse_state *parse_state);
t_node* parse_simple_cmd_tail(t_parser *parser, t_parse_state *parse_state);
t_node* parse_command(t_parser *parser, t_parse_state *parse_state);
t_node* parse_subshell(t_parser *parser, t_parse_state *parse_state);
t_node* parse_list(t_parser *parser, t_parse_state *parse_state);
t_node* parse_list_tail(t_parser *parser, t_parse_state *parse_state);
t_node* parse_pipeline(t_parser *parser, t_parse_state *parse_state);
t_node* parse_pipeline_tail(t_parser *parser, t_parse_state *parse_state);
t_node* parse_err(t_parser *parser, t_parse_state *parse_state);

// ==================== 5th =====================
// t_parse_state	*init_transition_table(void)
// {
// 	static t_parse_state	transition_table[PARSE_STATES_CNT][TOKEN_TYPES_CNT] = {
// 		[WORD] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[ENV_VAR] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = ERR,
// 			[TOKEN_OR] = ERR,
// 			[TOKEN_AND] = ERR,
// 			[TOKEN_REDIR_IN] = ERR,
// 			[TOKEN_HEREDOC] = ERR,
// 			[TOKEN_REDIR_OUT] = ERR,
// 			[TOKEN_APPEND] = ERR,
// 			[TOKEN_DOLLAR_SIGN] = ERR,
// 			[TOKEN_L_PAREN] = ERR,
// 			[TOKEN_R_PAREN] = ERR,
// 			[TOKEN_SUBSHELL] = ERR,
// 			[TOKEN_ENV_VAR] = ERR,
// 			[TOKEN_WILDCARD] = ERR,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = ERR,
// 			[TOKEN_DQ_STR] = ERR,
// 			[TOKEN_OPERATOR] = ERR,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[WORD_LIST] = {
// 			[TOKEN_WORD] = WORD_LIST_TAIL,
// 			// [TOKEN_UNKNOWN] = ERR,
// 			// [TOKEN_PIPE] = PIPELINE,
// 			// [TOKEN_OR] = LIST_TAIL,
// 			// [TOKEN_AND] = LIST_TAIL,
// 			// [TOKEN_REDIR_IN] = REDIR,
// 			// [TOKEN_HEREDOC] = REDIR,
// 			// [TOKEN_REDIR_OUT] = REDIR,
// 			// [TOKEN_APPEND] = REDIR,
// 			// [TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			// [TOKEN_L_PAREN] = SUBSHELL,
// 			// [TOKEN_R_PAREN] = LIST,
// 			// [TOKEN_SUBSHELL] = LIST,
// 			// [TOKEN_ENV_VAR] = ENV_VAR,
// 			// [TOKEN_WILDCARD] = ERR,
// 			// [TOKEN_WHITESPACE] = ERR,
// 			// [TOKEN_SQ_STR] = WORD_LIST,
// 			// [TOKEN_DQ_STR] = WORD_LIST,
// 			// [TOKEN_OPERATOR] = ERR,
// 			// [TOKEN_ERROR] = ERR,
// 		},
// 		[WORD_LIST_TAIL] = {
// 			[TOKEN_WORD] = WORD_LIST_TAIL,
// 			// [TOKEN_UNKNOWN] = ERR,
// 			// [TOKEN_PIPE] = PIPELINE,
// 			// [TOKEN_OR] = LIST_TAIL,
// 			// [TOKEN_AND] = LIST_TAIL,
// 			// [TOKEN_REDIR_IN] = REDIR,
// 			// [TOKEN_HEREDOC] = REDIR,
// 			// [TOKEN_REDIR_OUT] = REDIR,
// 			// [TOKEN_APPEND] = REDIR,
// 			// [TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			// [TOKEN_L_PAREN] = SUBSHELL,
// 			// [TOKEN_R_PAREN] = LIST,
// 			// [TOKEN_SUBSHELL] = LIST,
// 			// [TOKEN_ENV_VAR] = WORD,
// 			// [TOKEN_WILDCARD] = WORD,
// 			// [TOKEN_WHITESPACE] = ERR,
// 			// [TOKEN_SQ_STR] = LIST,
// 			// [TOKEN_DQ_STR] = LIST,
// 			// [TOKEN_OPERATOR] = LIST,
// 			// [TOKEN_ERROR] = ERR,
// 		},
// 		[ASSIGN_WORD] = {
// 			[TOKEN_WORD] = WORD,
// 			// [TOKEN_UNKNOWN] = ERR,
// 			// [TOKEN_PIPE] = ERR,
// 			// [TOKEN_OR] = ERR,
// 			// [TOKEN_AND] = ERR,
// 			// [TOKEN_REDIR_IN] = ERR,
// 			// [TOKEN_HEREDOC] = ERR,
// 			// [TOKEN_REDIR_OUT] = ERR,
// 			// [TOKEN_APPEND] = ERR,
// 			// [TOKEN_DOLLAR_SIGN] = ERR,
// 			// [TOKEN_L_PAREN] = ERR,
// 			// [TOKEN_R_PAREN] = ERR,
// 			// [TOKEN_SUBSHELL] = ERR,
// 			// [TOKEN_ENV_VAR] = ERR,
// 			// [TOKEN_WILDCARD] = ERR,
// 			// [TOKEN_WHITESPACE] = ERR,
// 			// [TOKEN_SQ_STR] = ERR,
// 			// [TOKEN_DQ_STR] = ERR,
// 			// [TOKEN_OPERATOR] = ERR,
// 			// [TOKEN_ERROR] = ERR,
// 		},
// 		[REDIR] = {
// 			// [TOKEN_WORD] = WORD,
// 			// [TOKEN_UNKNOWN] = ERR,
// 			// [TOKEN_PIPE] = ERR,
// 			// [TOKEN_OR] = ERR,
// 			// [TOKEN_AND] = ERR,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			// [TOKEN_DOLLAR_SIGN] = ERR,
// 			// [TOKEN_L_PAREN] = ERR,
// 			// [TOKEN_R_PAREN] = ERR,
// 			// [TOKEN_SUBSHELL] = ERR,
// 			// [TOKEN_ENV_VAR] = ERR,
// 			// [TOKEN_WILDCARD] = ERR,
// 			// [TOKEN_WHITESPACE] = ERR,
// 			// [TOKEN_SQ_STR] = WORD,
// 			// [TOKEN_DQ_STR] = WORD,
// 			// [TOKEN_OPERATOR] = ERR,
// 			// [TOKEN_ERROR] = ERR,
// 		},
// 		[REDIR_LIST] = {
// 			// [TOKEN_WORD] = WORD,
// 			// [TOKEN_UNKNOWN] = ERR,
// 			// [TOKEN_PIPE] = ERR,
// 			// [TOKEN_OR] = ERR,
// 			// [TOKEN_AND] = ERR,
// 			[TOKEN_REDIR_IN] = REDIR_TAIL,
// 			[TOKEN_HEREDOC] = REDIR_TAIL,
// 			[TOKEN_REDIR_OUT] = REDIR_TAIL,
// 			[TOKEN_APPEND] = REDIR_TAIL,
// 			// [TOKEN_DOLLAR_SIGN] = ERR,
// 			// [TOKEN_L_PAREN] = ERR,
// 			// [TOKEN_R_PAREN] = ERR,
// 			// [TOKEN_SUBSHELL] = ERR,
// 			// [TOKEN_ENV_VAR] = ERR,
// 			// [TOKEN_WILDCARD] = ERR,
// 			// [TOKEN_WHITESPACE] = ERR,
// 			// [TOKEN_SQ_STR] = ERR,
// 			// [TOKEN_DQ_STR] = ERR,
// 			// [TOKEN_OPERATOR] = ERR,
// 			// [TOKEN_ERROR] = ERR,
// 		},
// 		[REDIR_TAIL] = {
// 			// [TOKEN_WORD] = WORD,
// 			// [TOKEN_UNKNOWN] = ERR,
// 			// [TOKEN_PIPE] = ERR,
// 			// [TOKEN_OR] = ERR,
// 			// [TOKEN_AND] = ERR,
// 			[TOKEN_REDIR_IN] = REDIR_TAIL,
// 			[TOKEN_HEREDOC] = REDIR_TAIL,
// 			[TOKEN_REDIR_OUT] = REDIR_TAIL,
// 			[TOKEN_APPEND] = REDIR_TAIL,
// 			// [TOKEN_DOLLAR_SIGN] = ERR,
// 			// [TOKEN_L_PAREN] = ERR,
// 			// [TOKEN_R_PAREN] = ERR,
// 			// [TOKEN_SUBSHELL] = ERR,
// 			// [TOKEN_ENV_VAR] = ERR,
// 			// [TOKEN_WILDCARD] = ERR,
// 			// [TOKEN_WHITESPACE] = ERR,
// 			// [TOKEN_SQ_STR] = ERR,
// 			// [TOKEN_DQ_STR] = ERR,
// 			// [TOKEN_OPERATOR] = ERR,
// 			// [TOKEN_ERROR] = ERR,
// 		},
// 		[SIMPLE_CMD_ELE] = {
// 			[TOKEN_WORD] = ASSIGN_WORD,
// 			// [TOKEN_UNKNOWN] = ERR,
// 			// [TOKEN_PIPE] = ERR,
// 			// [TOKEN_OR] = ERR,
// 			// [TOKEN_AND] = ERR,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ERR,
// 			// [TOKEN_L_PAREN] = ERR,
// 			// [TOKEN_R_PAREN] = ERR,
// 			// [TOKEN_SUBSHELL] = ERR,
// 			// [TOKEN_ENV_VAR] = ENV_VAR,
// 			// [TOKEN_WILDCARD] = ERR,
// 			// [TOKEN_WHITESPACE] = ERR,
// 			// [TOKEN_SQ_STR] = ERR,
// 			// [TOKEN_DQ_STR] = ERR,
// 			// [TOKEN_OPERATOR] = ERR,
// 			// [TOKEN_ERROR] = ERR,
// 		},
// 		[SIMPLE_CMD] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[SIMPLE_CMD_TAIL] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[CMD] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[SUBSHELL] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[LIST] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[LIST_TAIL] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[PIPELINE] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[PIPELINE_TAIL] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 		[ERR] = {
// 			[TOKEN_WORD] = WORD,
// 			[TOKEN_UNKNOWN] = ERR,
// 			[TOKEN_PIPE] = PIPELINE,
// 			[TOKEN_OR] = LIST_TAIL,
// 			[TOKEN_AND] = LIST_TAIL,
// 			[TOKEN_REDIR_IN] = REDIR,
// 			[TOKEN_HEREDOC] = REDIR,
// 			[TOKEN_REDIR_OUT] = REDIR,
// 			[TOKEN_APPEND] = REDIR,
// 			[TOKEN_DOLLAR_SIGN] = ENV_VAR,
// 			[TOKEN_L_PAREN] = SUBSHELL,
// 			[TOKEN_R_PAREN] = LIST,
// 			[TOKEN_SUBSHELL] = LIST,
// 			[TOKEN_ENV_VAR] = WORD,
// 			[TOKEN_WILDCARD] = WORD,
// 			[TOKEN_WHITESPACE] = ERR,
// 			[TOKEN_SQ_STR] = LIST,
// 			[TOKEN_DQ_STR] = LIST,
// 			[TOKEN_OPERATOR] = LIST,
// 			[TOKEN_ERROR] = ERR,
// 		},
// 	};

// 	return(transition_table);
// }
// ==================== 5th =====================

// ==================== 1st =====================
// typedef t_node* (*parser_fn)(t_token **, t_parse_state *);
// const parser_fn parsers[PARSE_STATES_CNT - 1] = {
// 	parse_nonspace,
// 	parse_word,
// 	parse_env_var,
// 	parse_word_list,
// 	parse_word_list_tail,
// 	parse_assignment_word,
// 	parse_redir,
// 	parse_simple_cmd_element,
// 	parse_redir_list,
// 	parse_redir_list_tail,
// 	parse_simple_cmd,
// 	parse_simple_cmd_tail,
// 	parse_command,
// 	parse_subshell,
// 	parse_list,
// 	parse_list_tail,
// 	parse_pipeline,
// 	parse_pipeline_tail
// };

// const t_parse_state transition_table[PARSE_STATES_CNT - 1][TOKEN_TYPES_CNT] = {
// 	[NONSPACE] = { [TOKEN_WORD] = WORD, [TOKEN_UNKNOWN] = ERR, },
// 	[WORD] = {},
// 	[ENV_VAR] = {},
// 	[WORD_LIST] = {},
// 	[WORD_LIST_TAIL] = {},
// 	[ASSIGN_WORD] = {},
// 	[REDIR] = {},
// 	[SIMPLE_CMD_ELE] = {},
// 	[REDIR_LIST] = {},
// 	[REDIR_TAIL] = {},
// 	[SIMPLE_CMD] = {},
// 	[SIMPLE_CMD_TAIL] = {},
// 	[CMD] = {},
// 	[SUBSHELL] = {},
// 	[LIST] = {},
// 	[LIST_TAIL] = {},
// 	[PIPELINE] = {},
// 	[PIPELINE_TAIL] = {},
// };

// t_parse_state	update_parse_state(t_token_type cur_type, t_parse_state cur_state)
// {
// 	return (transition_table[cur_state][cur_type]);
// }
// ==================== 1st =====================

// ==================== 2nd =====================
// void	syntax_err(t_token *token)
// {
// 	write(STD_ERR, "Error at ", 10);
// 	write(STD_ERR, token->value, ft_strlen(token->value));
// 	write(STD_ERR, " \n", 2);

// }

// t_node	*parse_word_list(t_parser *parser, t_parse_state *state)
// {
	
// }

// t_node	*parse_simple_cmd(t_parser *parser, t_parse_state *state)
// {
// 	t_node	*cmd_node;

// 	cmd_node = create_node(AST_COMMAND);
// 	if (!cmd_node)
// 		return (0);
// 	// cmd_node->cmd_args = parse_word_list(tokens, state);
// 	if (!cmd_node->cmd_args)
// 		return (0);
// 	return (cmd_node);
// }

// t_node *parse(t_token *tokens, t_size num_tokens)
// {
//     t_node			*root;
// 	t_node			*node;
// 	t_token			*token;
//     t_parse_state	state;
// 	parser_fn		parser;
	
//     root = 0;
// 	state = CMD;
//     return (root);
// }
// ==================== 2nd =====================

// ==================== 3rd =====================
// t_node *parse_simple_cmd(t_token **token) {
//     t_node *node = create_node(AST_COMMAND);
//     if (!node) {
//         return NULL;
//     }
    
//     if ((*token)->type == TOKEN_WORD) {
//         // Parsing the command name.
//         node->value = (*token)->value;
//         (*token)++;
//     }
    
//     // Parsing command arguments if they exist.
//     while ((*token)->type == TOKEN_WORD) {
//         // Here you would add these arguments to the command node.
//         // You may want to use a linked list or a dynamic array to store these.
//         (*token)++;
//     }
    
//     return node;
// }

// t_node *parse_redir(t_token **token) {
//     t_node *node = create_node(AST_REDIR_OUT);
//     if (!node) {
//         return NULL;
//     }
    
//     // Parse '>' or '>>'
//     if ((*token)->type == TOKEN_REDIR_OUT || (*token)->type == TOKEN_APPEND) {
//         node->value = (*token)->value;
//         (*token)++;
//     }
    
//     // Parse the redirection target.
//     if ((*token)->type == TOKEN_WORD) {
//         node->right = parse_simple_cmd(token);
//     }
    
//     return node;
// }

// t_node *parse_command(t_token **token) {
//     if ((*token)->type == TOKEN_WORD) {
//         return parse_simple_cmd(token);
//     }
    
//     if ((*token)->type == TOKEN_REDIR_OUT || (*token)->type == TOKEN_APPEND) {
//         return parse_redir(token);
//     }
    
//     // Here you would add similar checks and calls to parsing functions for other types of commands.
    
//     return NULL;
// }

// t_node *parse_pipeline(t_token **token) {
//     t_node *node = parse_command(token);
//     if (!node) {
//         return NULL;
//     }
    
//     while ((*token)->type == TOKEN_PIPE) {
//         t_node *pipe_node = create_node(AST_PIPE);
//         if (!pipe_node) {
//             // You may want to free the previously allocated nodes here.
//             return NULL;
//         }
        
//         pipe_node->left = node;
//         (*token)++;
        
//         pipe_node->right = parse_command(token);
//         if (!pipe_node->right) {
//             // You may want to free the previously allocated nodes here.
//             return NULL;
//         }
        
//         node = pipe_node;
//     }
    
//     return node;
// }
// ==================== 3rd =====================

// ==================== 4th =====================
// parse_fn	**init_parse_table(void)
// {
// 	t_size	i;
// 	t_size	j;
// 	parse_fn	*parse_table;
// 	i = 0;

// 	// parse_table = ft_calloc(PARSE_STATES_CNT, sizeof(void*));
// 	parse_table = ft_calloc(PARSE_STATES_CNT, sizeof(parse_fn));
// 	if (!parse_table)
// 		return (0);
// 	while (i < PARSE_STATES_CNT)
// 	{
// 		j = 0;
// 		parse_table[i] = ft_calloc(TOKEN_TYPES_CNT, sizeof(void*));
// 		if (!parse_table[i])
// 		{
// 			for (j = 0; j < i; j++)
// 				free(parse_table[j]);
// 			free(parse_table);
// 			return (0);
// 		}
// 		while (j < TOKEN_TYPES_CNT)
// 		{
// 			parse_table[i * TOKEN_TYPES_CNT + j] = parse_err;
// 			// parse_table[i][j] = parse_err;
// 			j++;
// 		}
// 		i++;
// 	}
// 	parse_table[WORD][TOKEN_WORD] = parse_word;
// 	parse_table[ENV_VAR][TOKEN_DOLLAR_SIGN] = parse_env_var;
// 	parse_table[ASSIGN_WORD][TOKEN_WORD] = parse_assignment_word;
// 	parse_table[REDIR][TOKEN_REDIR_IN] = parse_redir;
// 	parse_table[REDIR][TOKEN_REDIR_OUT] = parse_redir;
// 	parse_table[REDIR][TOKEN_HEREDOC] = parse_redir;
// 	parse_table[REDIR][TOKEN_APPEND] = parse_redir;
// 	parse_table[SIMPLE_CMD_ELE][TOKEN_WORD] = parse_simple_cmd_element;
// 	parse_table[SIMPLE_CMD_ELE][TOKEN_DOLLAR_SIGN] = parse_simple_cmd_element;
// 	parse_table[SIMPLE_CMD_ELE][TOKEN_REDIR_IN] = parse_simple_cmd_element;
// 	parse_table[SIMPLE_CMD_ELE][TOKEN_REDIR_OUT] = parse_simple_cmd_element;
// 	parse_table[SIMPLE_CMD_ELE][TOKEN_HEREDOC] = parse_simple_cmd_element;
// 	parse_table[SIMPLE_CMD_ELE][TOKEN_APPEND] = parse_simple_cmd_element;
// 	parse_table[SIMPLE_CMD][TOKEN_WORD] = parse_simple_cmd;
// 	parse_table[SIMPLE_CMD][TOKEN_DOLLAR_SIGN] = parse_simple_cmd;
// 	parse_table[SIMPLE_CMD][TOKEN_REDIR_IN] = parse_simple_cmd;
// 	parse_table[SIMPLE_CMD][TOKEN_REDIR_OUT] = parse_simple_cmd;
// 	parse_table[SIMPLE_CMD][TOKEN_HEREDOC] = parse_simple_cmd;
// 	parse_table[SIMPLE_CMD][TOKEN_APPEND] = parse_simple_cmd;
// 	parse_table[CMD][TOKEN_WORD] = parse_command;
// 	parse_table[CMD][TOKEN_DOLLAR_SIGN] = parse_command;
// 	parse_table[CMD][TOKEN_REDIR_IN] = parse_command;
// 	parse_table[CMD][TOKEN_REDIR_OUT] = parse_command;
// 	parse_table[CMD][TOKEN_HEREDOC] = parse_command;
// 	parse_table[CMD][TOKEN_APPEND] = parse_command;
// 	parse_table[CMD][TOKEN_L_PAREN] = parse_command;
// 	parse_table[SUBSHELL][TOKEN_L_PAREN] = parse_subshell;
// 	parse_table[LIST][TOKEN_WORD] = parse_list;
// 	parse_table[LIST][TOKEN_DOLLAR_SIGN] = parse_list;
// 	parse_table[LIST][TOKEN_REDIR_IN] = parse_list;
// 	parse_table[LIST][TOKEN_REDIR_OUT] = parse_list;
// 	parse_table[LIST][TOKEN_HEREDOC] = parse_list;
// 	parse_table[LIST][TOKEN_APPEND] = parse_list;
// 	parse_table[LIST][TOKEN_L_PAREN] = parse_list;
// 	parse_table[LIST_TAIL][TOKEN_AND] = parse_list_tail;
// 	parse_table[LIST_TAIL][TOKEN_OR] = parse_list_tail;
// 	parse_table[PIPELINE][TOKEN_WORD] = parse_pipeline;
// 	parse_table[PIPELINE][TOKEN_REDIR_IN] = parse_pipeline;
// 	parse_table[PIPELINE][TOKEN_REDIR_OUT] = parse_pipeline;
// 	parse_table[PIPELINE][TOKEN_HEREDOC] = parse_pipeline;
// 	parse_table[PIPELINE][TOKEN_APPEND] = parse_pipeline;
// 	parse_table[PIPELINE][TOKEN_L_PAREN] = parse_pipeline;
// 	parse_table[PIPELINE][TOKEN_DOLLAR_SIGN] = parse_pipeline;
// 	parse_table[PIPELINE_TAIL][TOKEN_PIPE] = parse_pipeline_tail;
// 	return (parse_table);
// }

// t_bool	check(t_parser *parser, t_token_type type)
// {
// 	if (parser->cur >= parser->num_tokens)
//         return FALSE;
// 	return (parser->tokens[parser->cur].type == type);
// }

// void	advance(t_parser *parser)
// {
// 	if (parser->cur < parser->num_tokens)
// 		parser->cur++;
// }

// t_node *parse(t_token *tokens, t_size num_tokens) {

// 	t_parser	parser;
// 	t_parse_state	state;

// 	parser.tokens = tokens;
// 	parser.cur = 0;
// 	parser.num_tokens = num_tokens;
// 	// parser.parse_table = init_parse_table();
// 	parser.advance = &advance;
// 	parser.check = &check;
// 	state = LIST;
//     return (parser.parse_table[state][parser.tokens[parser.cur].type](&parser, &state));
// }

// ==================== 4th =====================