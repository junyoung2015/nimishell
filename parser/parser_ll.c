/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 19:05:20 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/20 21:47:11 by jusohn           ###   ########.fr       */
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

	<WORD-LIST-TAIL> -> <WORD> <WORD-LIST-TAIL>
	<WORD-LIST-TAIL> -> ε

	<ASSIGNMENT-WORD> -> <WORD> = <WORD>

	<REDIRECTION> -> > <WORD>
	<REDIRECTION> -> < <WORD>
	<REDIRECTION> -> >> <WORD>
	<REDIRECTION> -> << <WORD>

	<SIMPLE-COMMAND-ELEMENT> -> <WORD>
	<SIMPLE-COMMAND-ELEMENT> -> <ASSIGNMENT-WORD>
	<SIMPLE-COMMAND-ELEMENT> -> <REDIRECTION>
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
	?<PIPELINE> -> <PIPELINE-TAIL>?

	<PIPELINE-TAIL> -> | <COMMAND> <PIPELINE-TAIL>
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
	ROW9	TOKEN_DOLLAR_SIGN
	ROW10	TOKEN_L_PAREN
	ROW11	TOKEN_WILDCARD
	ROW12	TOKEN_SQ_STR
	ROW13	TOKEN_DQ_STR
 */
#define ROW1 "\2\5\6\6\4\4\4\4\1\0\2\2\2"
#define ROW2 "\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define ROW3 "\5\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW4 "\5\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW5 "\3\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW6 "\3\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW7 "\3\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW8 "\3\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW9 "\1\0\0\0\0\0\0\0\3\0\0\0\0"
#define ROW10 "\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define ROW11 "\2\5\6\6\4\4\4\4\2\0\0\2\2"
#define ROW12 "\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define ROW13 "\2\5\6\6\4\4\4\4\2\0\2\2\2"

t_node *parse_word_list(t_parser *parser);
t_node *parse_word_list_tail(t_parser *parser);

t_node *parse_env_var(t_parser *parser);
t_node *parse_word(t_parser *parser);
t_node *parse_assign_word(t_parser *parser);
t_node *parse_redir(t_parser *parser);
t_node *parse_simple_cmd_element(t_parser *parser);
t_node *parse_redir_list(t_parser *parser);
t_node *parse_redir_list_tail(t_parser *parser);
t_node *parse_simple_cmd(t_parser *parser);
t_node *parse_simple_cmd_tail(t_parser *parser);
t_node *parse_command(t_parser *parser);
t_node *parse_subshell(t_parser *parser);
t_node *parse_list(t_parser *parser);
t_node *parse_list_tail(t_parser *parser);
t_node *parse_pipeline(t_parser *parser);
t_node *parse_pipeline_tail(t_parser *parser);
t_node *parse_err(t_parser *parser);

// type for function pointer array
typedef t_node *(*parse_fn)(t_parser *parser);

/**
 * @brief Check the current token matches with t_token_type passed. This is a
 * 		syntax sugar function.
 *
 * @param parser	parser struct
 * @param type		type of the token to compare
 * @return t_bool
 */
t_bool check(t_parser *parser, t_token_type type)
{
	if (parser->cur >= parser->size)
		return FALSE;
	return (parser->tokens[parser->cur].type == type);
}

/**
 * @brief Return the type of the token right after the current index. If
 * 		current token is the last one, return TOKENS_TYPES_CNT(20).
 *
 * @param parser		parser struct
 * @return t_token_type	type of the next token
 */
t_token_type peek(t_parser *parser)
{
	if (parser->cur + 1 < parser->size)
		return (parser->tokens[parser->cur + 1].type);
	return (TOKEN_TYPES_CNT);
}

/**
 * @brief Increase the current index, after checking the range.
 *
 * @param parser	parser struct
 */
void advance(t_parser *parser)
{
	if (parser->cur < parser->size)
		parser->cur++;
}

/**
 * @brief Free the parsing table passed.
 *
 * @param table	2D table array for parsing table
 */
void free_table(char **table)
{
	t_size i;

	i = 0;
	while (i < 13)
	{
		free(table[i]);
		i++;
	}
	free(table);
}

/**
 * @brief Initialize the parsing table
 *
 * @return char** 2D array of parsing table
 */
char **init_rule_table(void)
{
	char **table;

	table = ft_calloc(13, sizeof(char));
	if (!table)
		return (0);
	table[0] = ROW1;
	table[1] = ROW2;
	table[2] = ROW3;
	table[3] = ROW4;
	table[4] = ROW5;
	table[5] = ROW6;
	table[6] = ROW7;
	table[7] = ROW8;
	table[8] = ROW9;
	table[9] = ROW10;
	table[10] = ROW11;
	table[11] = ROW12;
	table[12] = ROW13;

	return (table);
}

/**
 * @brief Update the parse state, to decide which parse function to call.
 *
 * @param table			char ** 2D parsing table
 * @param parser		parser struct
 * @param parse_state	current state of the parser
 */
void update_p_state(char **table, t_parser *parser, t_parse_state *parse_state)
{
	t_token cur;
	t_token_type peek;

	if (parser->cur == 0)
	{
		*parse_state = CMD;
		return;
	}
	cur = parser->tokens[parser->cur];
	peek = parser->peek(parser);
	if (peek == TOKEN_TYPES_CNT && (*parse_state == WORD_LIST_TAIL || *parse_state == REDIR_TAIL || *parse_state == LIST_TAIL || *parse_state == PIPELINE_TAIL))
	{
		// already at the end of the tokens array
		return;
	}
	*parse_state = table[cur.type][peek];
}

/**
 * @brief Parse function that generates an Error node. This indicates the
 * 		the location of syntax error.
 *
 * @param parser 	parser struct
 * @return t_node*	error node, indicating an error in syntax
 */
t_node *parse_err(t_paresr *parser)
{
	t_node *err_node;

	return (err_node);
}

/**
 * @brief Parse function for <SIMPLE-COMMAND-ELEMENT>, calling either <WORD>
 * 		or <REDIRECTION>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of the <SIMPLE-COMMAND-ELEMENT>
 */
t_node *parse_simple_cmd_element(t_parser *parser)
{
	t_node *cmd_element;

	return (cmd_element);
}

/**
 * @brief Parse function for <SIMPLE-COMMAND>, calling parse function
 *		<SIMPLE-COMMAND-ELEMENT>
 *
 * @param parser 	parser struct
 * @return t_node*	root node of the <SIMPLE-COMMAND>
 */
t_node *parse_simple_command(t_parser *parser)
{
	t_node *cmd_node;
	char *cmd_arg;

	cmd_node = create_node(AST_COMMAND);
	if (!cmd_node)
		return (0);
	cmd_node->cmd_args = ft_calloc(parser->size - parser->cur + 1, sizeof(char *));
	if (!cmd_node->cmd_args)
	{
		free(cmd_node);
		return (0);
	}
	while (parser->cur < parser->size && (parser->tokens[parser->cur].type == TOKEN_WORD || parser->tokens[parser->cur].type == TOKEN_SQ_STR || parser->tokens[parser->cur].type == TOKEN_DQ_STR))
	{
		cmd_arg = parse_simple_cmd_element(parser);
		if (!cmd_arg)
		{
			for (t_size i = 0; i < cmd_node->num_args; i++)
				free(cmd_node->cmd_args[i++]);
			free(cmd_node);
			return (0);
		}
		cmd_node->cmd_args[cmd_node->num_args] = cmd_arg;
		cmd_node->num_args++;
		parser->advance(parser);
	}
	return (cmd_node);
}

/**
 * @brief Parse function for <COMMAND>, calling <SIMPLE-COMMAND> or
 * 		<SUBSHELL>
 *
 * @param parser	parser struct
 * @return t_node*	root node from parse_simple_command or parse_subshell
 */
t_node *parse_command(t_parser *parser)
{
	t_node *cmd_node;
	t_token_state state;

	state = parser->peek(parser);
	if (TOKEN_L_PAREN == state)
	{
		cmd_node = parse_subshell(parser);
		if (!cmd_node)
			return (0);
	}
	else if (TOKEN_WORD == state || TOKEN_REDIR_IN == state || TOKEN_REDIR_OUT == state || TOKEN_HEREDOC == state || TOKEN_APPEND == state)
	{
		cmd_node = parse_simple_cmd(parser);
		if (!cmd_node)
			return (0);
	}
	else
	{
		// err
		cmd_node = create_node(AST_ERR);
		if (!cmd_node)
			return (0);
		cmd_node->cmd_args = ft_calloc(1, sizeof(char *));
		if (!cmd_node->cmd_args)
			return (0);
		cmd_node->cmd_args[0] = parser->tokens[parser->cur].value;
	}
	return (cmd_node);
}

/**
 * @brief Parse function for <WORD>. For <REDIRECTION>, <WORD> should be
 * 		attached to left (<, <<) node or right (>, >>) of the parent node.
 * 		For	it is an argument, so should be directly attached to
 *		parent->cmd_args[0 < i].
 *		If previous token is <WORD>, it should be considered as arguments,
 *		attaching arugments to (parser->tmp->cmd_args[0 < i]) and return
 *		(parser->tmp).
 *
 * @param parser	paresr struct
 * @return t_node*	root node of the <REDIRECTION>, or <WORD>
 */
t_node *parse_word(t_parser *parser)
{
	t_node *word_node;

	return (word_node);
}

/**
 * @brief Parse function for <REDIRECTION-LIST-TAIL>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of <REDIRECTION-LIST-TAIL>
 */
t_node *parse_redir_tail(t_parser *parser)
{
	t_node *redir_tail_node;

	return (redir_tail_node);
}

/**
 * @brief Parse function for <REDIRECTION>. '<, >, >>, <<' should be the root
 *		node, and filename should be at the left node (<, <<) or right (>, >>).
 *
 * @param parser 	paresr struct
 * @return t_node*	root node of the <REDIRECTION>
 */
t_node *parse_redir(t_parser *parser)
{
	t_node *redir_node;

	return (redir_node);
}

/**
 * @brief Parse function for <REDIRECTION-LIST>
 *
 * @param parser	parser struct
 * @return t_node*	root node from parse_redirection
 */
t_node *parse_redir_list(t_parser *parser)
{
	t_node *redir_node;

	retunr(redir_node);
}

/**
 * @brief Parse function for <PARSE-PIPELINE-TAIL>, calling <COMMAND> and
 * 		<PIPELINE-TAIL>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of <PIPELINE-TAIL>, '|'.
 */
t_node *parse_pipeline_tail(t_parser *parser)
{
	t_node *pipeline_tail_node;

	return (pipeline_tail_node);
}

/**
 * @brief Parse function for <PIPELINE>, calling <COMMAND> or <PIPELINE-TAIL>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of <PIPELINE>
 */
t_node *parse_pipeline(t_parser *parser)
{
	t_node *pipe_node;

	return (pipe_node);
}

/**
 * @brief Parse function for <LIST-TAIL>, calling <PIPELINE> and <LIST-TAIL>
 * 		'&&' or '||' becomes a root node, <PIPELINE> before '&&' and '||'
 *		operator becomes a left child, and <PIPELINE> after the '&&' and '||'
 *		becomes a right child.
 *
 * @param parser	parser struct
 * @return t_node*	root node with '&&' or '||'
 */
t_node *parse_list_tail(t_parser *parser)
{
	t_node *list_tail_node;

	retunr(list_tail_node);
}

/**
 * @brief Parse function for <LIST>, calling <PIPELINE> and <LIST-TAIL>
 *
 * @param parser	parser struct
 * @return t_node*	root node of <LIST>
 */
t_node *parse_list(t_parser *parser)
{
	t_node *list_node;

	return (list_node);
}

/**
 * @brief Parse function for <SUBSHELL>, calling <LIST>
 *
 * @param parser	parser struct
 * @return t_node*	root node of <SUBSHELL>
 */
t_node *parse_subshell(t_parser *parser)
{
	t_node *subshell_node;

	return (subshell_node);
}

/**
 * @brief LL(1) recursive decent parser, which receives array of t_token
 * 		and generate an AST (Abstract Syntax Tree).
 *
 * @param tokens		array of tokens after lexical analysis
 * @param num_tokens	length of the array
 * @return t_node*		root node of AST generated
 */
t_node *parse_tokens_ll(t_token *tokens, t_size num_tokens)
{
	t_parse_state parse_state;
	t_node *root;
	t_node *new_node;
	t_parser parser;
	char **table;
	const parse_fn parse_fn_array[8] = {
		// 0,
		// 0,
		// parse_command,
		// 0,
		// 0,
		// 0,
		// 0,
		// 0,
		parse_err,
		parse_env_var,
		parse_command,
		parse_word,
		parse_redir_list,
		parse_pipeline,
		parse_list,
		parse_subshell,
	};

	root = 0;
	parse_state = CMD;
	parser.tokens = tokens;
	parser.size = num_tokens;
	parser.cur = 0;
	parser.tmp = root;
	parser.check = &check;
	parser.advance = &advance;
	parser.peek = &peek;
	table = init_rule_table();
	if (!table)
		return (0);
	while (parser.cur < parser.size)
	{
		update_p_state(table, &parser, &parse_state);
		if (parse_state == ERR)
		{
			// deal with err and free?
			return 0;
		}
		new_node = parse_fn_array[parse_state](&parser);
		if (new_node != 0)
		{
			if (root == 0)
				root = new_node;
			else
			{
				append_child_node(root, new_node);
			}
		}
		else
		{
			return (0);
			// syntax err or malloc err?
		}
		parser.advance(&parser);
	}
	free_table(table);
	return (root);
}
