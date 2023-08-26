/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 19:05:20 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/23 17:15:43 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Production rule, in BNF grammar form
	========== NOTE ==========
	// {} : 0 or more occurrences
	// [] : denotes 0 or 1 occurrence
	//  | : "OR"
	// ?: not sure
	<NONSPACE> -> any character satisfying the condition !isspace(character)

	<WORD> -> <NONSPACE>

	<ENV-VAR> -> $ <WORD>

	<WORD-LIST> -> <WORD> <WORD-LIST-TAIL>

	<WORD-LIST-TAIL> -> <WORD> <WORD-LIST-TAIL>  // advance() in this case
	<WORD-LIST-TAIL> -> ε

	<ASSIGNMENT-WORD> -> <WORD> = <WORD>

	<REDIRECTION> -> > <WORD-LIST>	 // advance() in this case
	<REDIRECTION> -> < <WORD-LIST>	 // advance() in this case
	<REDIRECTION> -> >> <WORD-LIST>	 // advance() in this case
	<REDIRECTION> -> << <WORD-LIST>	 // advance() in this case

	<SIMPLE-COMMAND-ELEMENT> -> <WORD-LIST>
	<SIMPLE-COMMAND-ELEMENT> -> <ASSIGNMENT-WORD>
	<SIMPLE-COMMAND-ELEMENT> -> <REDIRECTION-LIST>
	<SIMPLE-COMMAND-ELEMENT> -> <ENV-VAR>

	<REDIRECTION-LIST> -> <REDIRECTION> <REDIRECTION-LIST-TAIL>
	?<REDIRECTION-LIST> -> <REDIRECTION-LIST-TAIL>?

	<REDIRECTION-LIST-TAIL> -> <REDIRECTION> <REDIRECTION-LIST-TAIL>
	<REDIRECTION-LIST-TAIL> -> ε

	<SIMPLE-COMMAND> -> <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>

	<SIMPLE-COMMAND-TAIL> -> <SIMPLE-COMMAND-ELEMENT> <SIMPLE-COMMAND-TAIL>
	?<SIMPLE-COMMAND-TAIL> -> ε?

	<COMMAND> -> <SIMPLE-COMMAND>
	<COMMAND> -> <SUBSHELL>
	<COMMAND> -> <SUBSHELL> <REDIRECTION-LIST>
	?<COMMAND> -> <SUBSHELL> <PIPELINE>?

	<SUBSHELL> -> ( <LIST> )

	<LIST> -> <PIPELINE> <LIST-TAIL>

	<LIST-TAIL> -> && <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> || <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> ε

	<PIPELINE> -> <COMMAND> <PIPELINE-TAIL>
	?<PIPELINE> -> <PIPELINE-TAIL>?i

	?<PIPELINE-TAIL> -> | <PIPELINE>?				// advance() in this case
	 <PIPELINE-TAIL> -> | <COMMAND> <PIPELINE-TAIL>	// advance() in this case
	<PIPELINE-TAIL> -> ε
 */

/*
	Rules for parsing table:
	0		ERR
	1		ENV_VAR
	2		CMD
	3		WORD
	4		REDIR_LIST
	5		PIPELINE
	6		LIST
	7		SUBSHELL

	ROW1	TOKEN_WORD
	ROW2	TOKEN_PIPE
	ROW3	TOKEN_OR
	ROW4	TOKEN_AND
	ROW5	TOKEN_REDIR_IN
	ROW6	TOKEN_HEREDOC
	ROW7	TOKEN_REDIR_OUT
	ROW8	TOKEN_APPEND
	ROW9	TOKEN_DOLLAR_SIGN // TODO: remove this when refactoring
	ROW10	TOKEN_L_PAREN
	ROW12	TOKEN_SQ_STR
	ROW13	TOKEN_DQ_STR
	ROW14	TOKEN_OPERATOR
 */

#define ROW1	"\2\5\6\6\4\4\4\4\1\0\2\2\2"
#define ROW11	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define ROW12	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define ROW13	"\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW2	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define ROW3	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW4	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW5	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW6	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW7	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW8	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW9	"\1\0\0\0\0\0\0\0\3\0\0\0\0"
#define ROW10	"\2\0\0\0\0\4\0\0\1\7\0\2\2"

/* NEW NEW NEW NEW NEW
	Rules for parsing table:
	0		ERR
	1		CMD
	2		WORD
	3		REDIR_LIST
	4		PIPELINE
	5		LIST
	6		SUBSHELL

	ROW1	TOKEN_WORD
	ROW2	TOKEN_PIPE
	ROW3	TOKEN_OR
	ROW4	TOKEN_AND
	ROW5	TOKEN_REDIR_IN
	ROW6	TOKEN_HEREDOC
	ROW7	TOKEN_REDIR_OUT
	ROW8	TOKEN_APPEND
	ROW9	TOKEN_L_PAREN
	ROW10	TOKEN_SQ_STR
	ROW11	TOKEN_DQ_STR
	ROW12	TOKEN_OPERATOR
 */

// #define ROW1	"\1\4\5\5\3\3\3\3\0\1\1"
// #define ROW10	"\2\4\5\5\3\3\3\3\0\1\1"
// #define ROW11	"\2\4\5\5\3\3\3\3\0\1\2"
// #define ROW12	"\0\0\0\0\0\0\0\0\0\0\0"
// #define ROW2	"\1\0\0\0\0\3\0\0\6\1\1"
// #define ROW3	"\5\0\0\0\0\0\0\0\6\0\0"
// #define ROW4	"\5\0\0\0\0\0\0\0\6\0\0"
// #define ROW5	"\2\0\0\0\0\0\0\0\0\0\0"
// #define ROW6	"\2\0\0\0\0\0\0\0\0\0\0"
// #define ROW7	"\2\0\0\0\0\0\0\0\0\0\0"
// #define ROW8	"\2\0\0\0\0\0\0\0\0\0\0"
// #define ROW9	"\1\0\0\0\0\3\0\0\6\1\1"

/**
 * @brief Initialize the parsing table
 *
 * @return char** table of parsing table
 */
char **init_parser(t_node **root, t_parser *parser, t_token *tokens, t_size num_tokens)
{
	char **table;

	*root = 0;
	parser->tokens = tokens;
	parser->size = num_tokens;
	parser->cur = 0;
	table = ft_calloc(14, sizeof(char *));
	if (!table)
		return (0);
	table[0] = ROW1;
	table[1] = ROW11;
	table[2] = ROW12;
	table[3] = ROW13;
	table[4] = ROW2;
	table[5] = ROW3;
	table[6] = ROW4;
	table[7] = ROW5;
	table[8] = ROW6;
	table[9] = ROW7;
	table[10] = ROW8;
	table[11] = ROW9;
	table[12] = ROW10;
	return (table);
}

/**
 * @brief LL(1) recursive decent parser, which receives array of t_token
 * 		and generate an AST (Abstract Syntax Tree).
 *
 * @param tokens		array of tokens after lexical analysis
 * @param num_tokens	length of the array
 * @return t_node*		root node of AST generated
 */
t_node *parse_tokens(t_token *tokens, t_size num_tokens)
{
	t_parse_state	parse_state;
	t_node			*root;
	t_node			*node;
	t_parser		parser;
	char			**table;
	const t_parse	fn_arr[8] = {p_err,p_cmd,p_redir_l,p_pipe_l,p_l,p_sub};

	table = init_parser(&root, &parser, tokens, num_tokens);
	if (!table)
		return (0);
	while (parser.cur < parser.size)
	{
		update_p_state(table, &parser, &parse_state);
		if (parse_state == PARSE_STATES_CNT)
			break ;
		node = fn_arr[parse_state](&parser, root);
		if (node != 0)
		{
			if (check_err_node(node))
			{
				if (root && root != node)
					free_ast(root);
				free(table);
				free_ast(node);
				return (0);
			}
			if (root == 0)
				root = node;
			else if (AST_REDIR_IN <= node->type && AST_REDIR_APPEND <= node->type)
				append_redir_node(root, node);
			else
				append_child_node(root, node);
		}
		else
		{
			if (root)
				free_ast(root);
			free(table);
			return (0);
		}
	}
	free(table);
	return (root);
}
