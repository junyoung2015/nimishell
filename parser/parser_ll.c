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
	?<PIPELINE> -> <PIPELINE-TAIL>?

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
	ROW11	TOKEN_WILDCARD
	ROW12	TOKEN_SQ_STR
	ROW13	TOKEN_DQ_STR
	ROW14	TOKEN_OPERATOR
 */

#define ROW1	"\2\5\6\6\4\4\4\4\1\0\2\2\2"
#define ROW12	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define ROW13	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
#define ROW14	"\0\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW2	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define ROW3	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW4	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
#define ROW5	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW6	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW7	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW8	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
#define ROW9	"\1\0\0\0\0\0\0\0\3\0\0\0\0"
#define ROW10	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
#define ROW11	"\2\5\6\6\4\4\4\4\2\0\0\2\2"

// #define ROW1	"\2\5\6\6\4\4\4\4\1\0\2\2\2"
// #define ROW2	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
// #define ROW3	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
// #define ROW4	"\6\0\0\0\0\0\0\0\0\7\0\0\0"
// #define ROW5	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
// #define ROW6	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
// #define ROW7	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
// #define ROW8	"\4\0\0\0\0\0\0\0\0\0\0\0\0"
// #define ROW9	"\1\0\0\0\0\0\0\0\3\0\0\0\0"
// #define ROW10	"\2\0\0\0\0\4\0\0\1\7\0\2\2"
// #define ROW11	"\2\5\6\6\4\4\4\4\2\0\0\2\2"
// #define ROW12	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
// #define ROW13	"\2\5\6\6\4\4\4\4\2\0\2\2\2"
// #define ROW14	"\0\0\0\0\0\0\0\0\0\0\0\0\0"


char	*parse_word(t_parser *parser);
t_node	*parse_word_list(t_parser *parser, t_node *parent);
t_node	*parse_redir(t_parser *parser, t_node *parent);
t_node	*parse_redir_list_tail(t_parser *parser, t_node *parent);
t_node	*parse_redir_list(t_parser *parser, t_node *parent);
t_node	*parse_simple_cmd_element(t_parser *parser, t_node *parent);
t_node	*parse_simple_cmd_tail(t_parser *parser, t_node *parent);
t_node	*parse_simple_cmd(t_parser *parser, t_node *parent);
t_node	*parse_command(t_parser *parser, t_node *parent);
t_node	*parse_subshell(t_parser *parser, t_node *parent);
t_node	*parse_list_tail(t_parser *parser, t_node *parent);
t_node	*parse_list(t_parser *parser, t_node *parent);
t_node	*parse_pipeline_tail(t_parser *parser, t_node *parent);
t_node	*parse_pipeline(t_parser *parser, t_node *parent);
t_node	*parse_err(t_parser *parser, t_node *parent);

// type for function pointer array
typedef t_node *(*t_parser_fn)(t_parser *parser, t_node *parent);

/**
 * @brief Check the type of the current token.
 * 
 * @param parser 		parser struct
 * @return t_token_type	type of the current token
 */
t_token_type	cur_type(t_parser *parser)
{
	if (parser->cur < parser->size)
		return (parser->tokens[parser->cur].type);
	return (TOKEN_TYPES_CNT);
}

/**
 * @brief Check the current token matches with t_token_type passed.
 *
 * @param parser	parser struct
 * @param type		type of the token to compare
 * @return t_bool
 */
t_bool check(t_parser *parser, t_token_type type)
{
	if (parser->cur >= parser->size)
		return (FALSE);
	return (parser->cur_type(parser) == type);
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
	if (parser->cur + 1 < parser->size || parser->cur < parser->size)
		return (parser->tokens[parser->cur + 1].type);
	return (TOKEN_TYPES_CNT);
}

/**
 * @brief Return the type of the current token, and increase the current
 *		index.
 * 
 * @param parser 		parser struct
 * @return t_token_type type of the next token
 */
t_token_type	consume(t_parser *parser)
{
	if (parser->cur < parser->size)
		return (parser->tokens[parser->cur++].type);
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
 * @brief Check whether current token is a word or not. SQ_STR and DQ_STR are
 * 		also considered as a word, like bash.
 * 
 * @param parser	parser struct
 * @return t_bool	TRUE if current token is a word, FALSE otherwise
 */
t_bool	is_word_token(t_parser *parser)
{
	t_token_type	type;

	type = parser->cur_type(parser);
	if (parser->cur < parser->size)
		return (type == TOKEN_WORD || type == TOKEN_SQ_STR || type == TOKEN_DQ_STR);
	return (FALSE);
}

/**
 * @brief Check whether current token is a redirection or not.
 * 
 * @param parser	parser struct
 * @return t_bool	TRUE if current token is a redirection, FALSE otherwise
 */
t_bool	is_redir_token(t_parser *parser)
{
	t_token_type	type;

	type = parser->cur_type(parser);
	if (parser->cur < parser->size)
		return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT || \
			type == TOKEN_APPEND || type == TOKEN_HEREDOC);
	return (FALSE);
}

/**
 * @brief Check whether the command (cmd_args[0]) is a BUILTIN or not.
 * 
 * @param node	node to check
 */
void	is_builtin_node(t_node *node)
{
	char	*str;

	str = node->cmd_args[0];
	if (!ft_strcmp(str, "echo"))
		node->builtin = ECHO_BUILTIN;
	if (!ft_strcmp(str, "cd"))
		node->builtin = CD;
	if (!ft_strcmp(str, "pwd"))
		node->builtin = PWD;
	if (!ft_strcmp(str, "export"))
		node->builtin = EXPORT;
	else if (!ft_strcmp(str, "unset"))
		node->builtin = UNSET;
	else if (!ft_strcmp(str, "env"))
		node->builtin = ENV;
	else if (!ft_strcmp(str, "exit"))
		node->builtin = EXIT;
}

/**
 * @brief Append the child (redirection node) to the parent node. If the
 *		parent node is NULL, return.
 * 
 * @param parent	parent node
 * @param child		child node to attach
 */
void	append_redir_node(t_node *parent, t_node *child)
{
	t_node	*tmp;

	if (!parent || !child)
		return ;
	if (child->type == AST_REDIR_IN || child->type == AST_HEREDOC)
	{
		if (!parent->left)
			parent->left = child;
		else
		{
			tmp = parent->left;
			while (tmp->sibling)
				tmp = tmp->sibling;
			tmp->sibling = child;
		}
	}
	else if (child->type == AST_REDIR_OUT || child->type == AST_REDIR_APPEND)
	{
		if (!parent->right)
			parent->right = child;
		else
		{
			tmp = parent->right;
			while (tmp->sibling)
				tmp = tmp->sibling;
			tmp->sibling = child;
		}
	}
}

/**
 * @brief Free the parsing table passed.
 *
 * @param table	table for parsing table
 */
void free_table(char **table)
{
	free(table);
}

/**
 * @brief Initialize the parsing table
 *
 * @return char** table of parsing table
 */
char **init_rule_table(void)
{
	char **table;

	table = ft_calloc(15, sizeof(char *));
	if (!table)
		return (0);
	table[0] = ROW1;
	table[1] = ROW12;
	table[2] = ROW13;
	table[3] = ROW14;
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
		*parse_state = LIST;
		return ;
	}
	cur = parser->tokens[parser->cur];
	peek = parser->peek(parser);
	// TODO: check the condition again, since we might not use word_list_tail
	// if (peek == TOKEN_TYPES_CNT || (*parse_state == WORD_LIST_TAIL || *parse_state == REDIR_TAIL || *parse_state == LIST_TAIL || *parse_state == PIPELINE_TAIL))
	// {
	// 	*parse_state = PARSE_STATES_CNT;
	// 	// already at the end of the tokens array
	// 	return ;
	// }
	*parse_state = table[cur.type][peek];
}

/**
 * @brief Traverse the AST tree in postorder, return the error node if it
 * 		exists.
 * 
 * @param node	node of the AST tree.
 * @param err_node	pointer to the first AST_ERR node found during traversal
 */
void	postorder_traversal(t_node *node, t_node **err_node)
{
    if (!node || (*err_node))
		return ;	
	if (node->left)
		postorder_traversal(node->left, err_node);
	if (node->right)
		postorder_traversal(node->right, err_node);
    if (node->type == AST_ERR)
        *err_node = node;
}

/**
 * @brief Check whether there is an AST_ERR node in the subtree rooted at
 * 		new_node. If so, print error message and return TRUE. Otherwise,
 * 		return FALSE.
 * 
 * @param new_node	root node of the subtree to check for errors
 * @return t_bool	TRUE if there is an error node, FALSE otherwise
 */
t_bool	check_err_node(t_node *new_node)
{
    t_node *err_node = NULL;
    if (!new_node)
        return (FALSE);
    postorder_traversal(new_node, &err_node);
    if (err_node)
    {
        write(STD_ERR, "minishell: syntax error near unexpected token `", 47);
        write(STD_ERR, err_node->cmd_args[0], ft_strlen(err_node->cmd_args[0]));
        write(STD_ERR, "`\n", 2);
        return (TRUE);
    }
    return (FALSE);
}

/**
 * @brief Parse function that generates an Error node. This indicates the
 * 		the location of syntax error.
 *
 * @param parser 	parser struct
 * @return t_node*	error node, indicating an error in syntax
 */
t_node *parse_err(t_parser *parser, t_node *parent)
{
	t_node	*err_node;
	(void)	parent;

	err_node = create_node(AST_ERR);
	if (!err_node)
		return (0);
	err_node->cmd_args = ft_calloc(1, sizeof(char *));
	if (!err_node->cmd_args)
		return (0);
	err_node->cmd_args[err_node->num_args++] = ft_strdup(parser->tokens[parser->cur].val);
	return (err_node);
}

/**
 * @brief Parse function for <SIMPLE-COMMAND-ELEMENT>, calling either <WORD>
 * 		or <REDIRECTION>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of the <SIMPLE-COMMAND-ELEMENT>
 */
t_node *parse_simple_cmd_element(t_parser *parser, t_node *parent)
{
	t_node	*cmd_element;

	if (parser->is_word(parser))
	{
		cmd_element = parse_word_list(parser, parent);
		if (!cmd_element)
			return (0);
		cmd_element->type = AST_CMD;
	}
	else if (parser->is_redir(parser))
	{
		cmd_element = parse_redir_list(parser, parent);
		if (!cmd_element)
			return (0);
	}
	else
	{
		// err handling
		cmd_element = parse_err(parser, parent);
	}
	return (cmd_element);
}

/**
 * @brief Parse function for <SIMPLE-COMMAND-TAIL>, calling
 * 		<SIMPLE-COMMAND-ELEMENT> and <SIMPLE-COMMAND-TAIL>.
 * 
 * @param parser 
 * @return t_node* 
 */
t_node	*parse_simple_cmd_tail(t_parser *parser, t_node *parent)
{
	t_node	*cmd_node;
	t_node	*simple_cmd_tail_node;

	simple_cmd_tail_node = 0;
	// Do I have to check the token type here again, when its already checked before?
	if (parser->is_word(parser) || parser->is_redir(parser))
	{
		simple_cmd_tail_node = parse_simple_cmd_element(parser, parent);
		if (!simple_cmd_tail_node)
			return (0);
		if (AST_REDIR_IN <= simple_cmd_tail_node->type && simple_cmd_tail_node->type <= AST_REDIR_APPEND) // appending to the first cmd when there is a redirection - ls | wc > a.txt, appending to ls here
			append_redir_node(parent, simple_cmd_tail_node);
		else
			append_child_node(parent, simple_cmd_tail_node);
		if (parser->is_word(parser) || parser->is_redir(parser))
		{
			cmd_node = parse_simple_cmd_tail(parser, simple_cmd_tail_node);
			if (!cmd_node)
				return (0);
			// append_child_node(simple_cmd_tail_node, cmd_node);
		}
	}
	return (simple_cmd_tail_node);
}

/**
 * @brief Parse function for <SIMPLE-COMMAND>, calling parse function for
 *		<SIMPLE-COMMAND-ELEMENT>
 *
 * @param parser 	parser struct
 * @return t_node*	root node of the <SIMPLE-COMMAND>
 */
t_node	*parse_simple_cmd(t_parser *parser, t_node *parent)
{
	t_node	*cmd_tail;
	t_node	*cmd_node;

	if (parser->is_word(parser) || parser->is_redir(parser))
	{
		cmd_node = parse_simple_cmd_element(parser, parent);
		if (!cmd_node)
			return (0);
		if (parser->is_word(parser) || parser->is_redir(parser))
		{
			cmd_tail = parse_simple_cmd_tail(parser, cmd_node);
			if (!cmd_tail)
				return (0);
//			if (AST_REDIR_IN <= cmd_tail->type && cmd_tail->type <= AST_REDIR_APPEND)
//				append_redir_node(cmd_node, cmd_tail);
//			else
//				append_child_node(cmd_node, cmd_tail);
		}
	}
	else
	{
		cmd_node = parse_err(parser, parent);
	}
	// parser->advance(parser);
	return (cmd_node);
}

/**
 * @brief Parse function for <COMMAND>, calling <SIMPLE-COMMAND> or
 * 		<SUBSHELL>
 *
 * @param parser	parser struct
 * @return t_node*	root node from parse_simple_cmd or parse_subshell
 */
t_node *parse_command(t_parser *parser, t_node *parent)
{
	t_node			*cmd_node;
	t_node			*redir_list_node;
	t_token_type	state;

	state = parser->cur_type(parser);
	if (TOKEN_L_PAREN == state)
	{
		cmd_node = parse_subshell(parser, parent);
		if (!cmd_node)
			return (0);
		if (parser->is_redir(parser))
		{
			redir_list_node = parse_redir_list(parser, parent);
			if (!redir_list_node)
				return (0);
			append_child_node(cmd_node, redir_list_node);
		}
	}
	else if (parser->is_word(parser) || parser->is_redir(parser))
	{
		cmd_node = parse_simple_cmd(parser, parent);
		if (!cmd_node)
			return (0);
	}
	else if (state == TOKEN_TYPES_CNT)
	{
		parser->cur--;
		cmd_node = parse_err(parser, parent);
	}
	else
	{
		// err handling
		cmd_node = parse_err(parser, parent);
	}
	return (cmd_node);
}

/**
 * @brief Parse function for <WORD>. For <REDIRECTION>, <WORD> should be
 * 		attached to left (<, <<) node or right (>, >>) of the parent node.
 * 		For	<WORD> as an argument of a command, so should be directly attached
 * 		to parent->cmd_args[0 < i].
 *		If previous token is <WORD>, it should be considered as arguments,
 *		attaching arugments to (parent->cmd_args[0 < i]) and return
 *		(parent).
 *
 * @param parser	paresr struct
 * @return t_node*	root node of the <REDIRECTION>, or <WORD>
 */
char *parse_word(t_parser *parser)
{
	char	*word;

	word = 0;
	if (parser->is_word(parser))
		word = ft_strdup(parser->tokens[parser->cur].val);
	return (word);
}

/**
 * @brief Parse function for <WORD-LIST-TAIL>, calling <WORD> until the next
 * 		token is not a <WORD>.
 * 
 * @param parser 	parser struct
 * @return t_node*	root node of the <WORD-LIST-TAIL>
 */
t_node	*parse_word_list(t_parser *parser, t_node *parent)
{
	t_node	*word_list_node;
	(void)	parent;

	word_list_node = create_node(AST_WORD_LIST);
	if (!word_list_node)
		return (0);
	word_list_node->cmd_args = ft_calloc(parser->size - parser->cur + 1, sizeof(char *));
	if (!word_list_node->cmd_args)
	{
		free(word_list_node);
		return (0);
	}
	while (parser->cur < parser->size && parser->is_word(parser))
	{
		word_list_node->cmd_args[word_list_node->num_args] = parse_word(parser);
		if (!word_list_node->cmd_args[word_list_node->num_args])
		{
			for (t_size i = 0; i < word_list_node->num_args; i++)
				free(word_list_node->cmd_args[i++]);
			free(word_list_node);
			return (0);
		}
		is_builtin_node(word_list_node);
		word_list_node->num_args++;
		parser->advance(parser);
	}
	return (word_list_node);
}

/**
 * @brief Parse function for <REDIRECTION>. '<, >, >>, <<' should be the root
 *		node, and filename should be at the left node (<, <<) or right (>, >>).
 *		Bash only consider the first redirection-in, ignoring the rest, and
 *		the last redirection-out. So, we should consider the same.
 *
 * @param parser 	paresr struct
 * @return t_node*	root node of the <REDIRECTION>
 */
t_node *parse_redir(t_parser *parser, t_node *parent)
{
	t_node			*redir_node;

	redir_node = 0;
	if (parser->check(parser, TOKEN_REDIR_IN))
	{
		parser->advance(parser);
		redir_node = parse_word_list(parser, parent);
		if (!redir_node)
			return (0);
		redir_node->type = AST_REDIR_IN;
	}
	else if (parser->check(parser, TOKEN_HEREDOC))
	{
		parser->advance(parser);
		redir_node = create_node(AST_HEREDOC);
		if (!redir_node)
			return (0);
		redir_node->cmd_args = ft_calloc(2, sizeof(char *));
		redir_node->cmd_args[redir_node->num_args] = parse_word(parser);
		redir_node->num_args = 1;
	}
	else if (parser->check(parser, TOKEN_REDIR_OUT))
	{
		parser->advance(parser);
		redir_node = parse_word_list(parser, parent);
		if (!redir_node)
			return (0);
		redir_node->type = AST_REDIR_OUT;
	}
	else if (parser->check(parser, TOKEN_APPEND))
	{
		parser->advance(parser);
		redir_node = parse_word_list(parser, parent);
		if (!redir_node)
			return (0);
		redir_node->type = AST_REDIR_APPEND;
	}
	// parser->advance(parser);
	return (redir_node);
}

/**
 * @brief Parse function for <REDIRECTION-LIST-TAIL>, calling <REDIRECTION> or
 *		indicate the end of the <REDIRECTION-LIST-TAIL>.
 * 
 * @param parser	parser struct
 * @return t_node*	root node of <REDIRECTION-LIST-TAIL>
 */
t_node	*parse_redir_list_tail(t_parser *parser, t_node *parent)
{
	t_node	*redir_node;
	t_node	*redir_list_tail_node;

	redir_list_tail_node = 0;
	if (parser->is_redir(parser))
	{
		redir_list_tail_node = parse_redir(parser, parent);
		if (!redir_list_tail_node)
			return (0);
		append_redir_node(parent, redir_list_tail_node);
		if (parser->is_redir(parser))
		{
			redir_node = parse_redir_list_tail(parser, redir_list_tail_node);
			if (!redir_node)
				return (0);
		}
	}
	return (redir_list_tail_node);
}

/**
 * @brief Parse function for <REDIRECTION-LIST>, calling <REDIRECTION> and
 * 		<REDIRECTION-LIST-TAIL>.
 * 
 * @param parser	parser struct
 * @return t_node*	root node of <REDIRECTION-LIST>
 */
t_node	*parse_redir_list(t_parser *parser, t_node *parent)
{
	t_node	*redir_list_node;
	t_node	*redir_list_tail_node;

	redir_list_node = parse_redir(parser, parent);
	if (!redir_list_node) // err?
		return (0);
	if (parser->is_redir(parser))
	{
		redir_list_tail_node = parse_redir_list_tail(parser, redir_list_node);
		if (!redir_list_tail_node) // err?
			return (0);
	}
	return (redir_list_node);
}

/**
 * @brief Parse function for <PARSE-PIPELINE-TAIL>, calling <COMMAND> and
 * 		<PIPELINE-TAIL>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of <PIPELINE-TAIL>, '|'.
 */
t_node *parse_pipeline_tail(t_parser *parser, t_node *parent)
{
	t_node	*right_node;
	t_node	*pipe_node;

	// do I need to check whether token == TOKEN_PIPE again?
	pipe_node = 0;
	if (parser->check(parser, TOKEN_PIPE))
	{
		parser->advance(parser);
		right_node = parse_pipeline(parser, parent);
		if (!right_node) // err?
			return (0);
		else if (right_node->type == AST_ERR)
			return (right_node);
		pipe_node = create_node(AST_PIPE);
		if (!pipe_node)	// malloc err
			return (0);
		pipe_node->right = right_node;
		return (pipe_node);
	}
	else	// syntax error
		return (0);
	return (pipe_node);
}

/**
 * @brief Parse function for <PIPELINE>, calling <COMMAND> or <PIPELINE-TAIL>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of <PIPELINE>
 */
t_node *parse_pipeline(t_parser *parser, t_node *parent)
{
	t_node	*cmd_node;
	// t_node	*right_node;
	t_node	*pipe_node;

	cmd_node = parse_command(parser, parent);
	if (!cmd_node) // err? no command before '|'
		return (0);
	if (cmd_node->type == AST_ERR)
		return (cmd_node);
	if (parser->check(parser, TOKEN_PIPE))
	{
		pipe_node = parse_pipeline_tail(parser, cmd_node);
		if (!pipe_node) // err?
			return (0);
		if (pipe_node->type == AST_ERR)
		{
			free_ast(cmd_node);
			return (pipe_node);
		}
		pipe_node->left = cmd_node;
		return (pipe_node);
	}
	return (cmd_node);
}

/**
 * @brief Parse function for <LIST-TAIL>, calling <PIPELINE> and <LIST-TAIL>
 * 		'&&' or '||' becomes a root node, <PIPELINE> before '&&' and '||'
 *		operator becomes a left child, and <PIPELINE> after the '&&' and '||'
 *		becomes a right child.
 *
	<LIST-TAIL> -> && <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> || <PIPELINE> <LIST-TAIL>
	<LIST-TAIL> -> ε
 * @param parser	parser struct
 * @return t_node*	root node with '&&' or '||'
 */
t_node *parse_list_tail(t_parser *parser, t_node *parent)
{
	t_node			*logic_node;
	t_node			*pipeline_node;
	t_node			*list_tail_node;
	t_node_type		state;
	(void)			parent;

	logic_node = 0;
	if (parser->check(parser, TOKEN_AND) || parser->check(parser, TOKEN_OR))
	{
		state = AST_AND;
		if (parser->check(parser, TOKEN_OR))
			state = AST_OR;
		logic_node = create_node(state);
		if (!logic_node)	// malloc err
			return (0);
		// logic_node->left = parent;
		// append_child_node(logic_node, parent);
		parser->advance(parser);
		pipeline_node = parse_pipeline(parser, logic_node);
		if (!pipeline_node)
		{
			// free and handle err here?
			free(logic_node);
			return (0);
		}
		logic_node->right = pipeline_node;
		if (pipeline_node->type == AST_ERR)
			return (logic_node);
		// logic_node->right = pipeline_node;
		// append_child_node(logic_node, pipeline_node);
		// append_child_node(logic_node, pipeline_node);
		if (parser->check(parser, TOKEN_AND) || parser->check(parser, TOKEN_OR))
		{
			list_tail_node = parse_list_tail(parser, pipeline_node);
			if (!list_tail_node) // err?
				return (0);
			append_child_node(pipeline_node, list_tail_node);
		}
		return (logic_node);
	}
	return (logic_node);
}

/**
 * @brief Parse function for <LIST>, calling <PIPELINE> and <LIST-TAIL>
 *
 * @param parser	parser struct
 * @return t_node*	root node of <LIST>
 */
t_node *parse_list(t_parser *parser, t_node *parent)
{
	t_node	*pipeline_node;
	t_node	*list_tail_node;

	pipeline_node = parse_pipeline(parser, parent);
	if (!pipeline_node) // err?
		return (0);
	if (parser->check(parser, TOKEN_AND) || parser->check(parser, TOKEN_OR))
	{
		list_tail_node = parse_list_tail(parser, pipeline_node);
		if (!list_tail_node) // err?
			return (0);
		append_child_node(list_tail_node, pipeline_node);
		// list_tail_node->left = pipeline_node;
		return (list_tail_node);
	}
	return (pipeline_node);
}

/**
 * @brief Parse function for <SUBSHELL>, calling <LIST>
 *
 * @param parser	parser struct
 * @return t_node*	root node of <SUBSHELL>
 */
t_node *parse_subshell(t_parser *parser, t_node *parent)
{
	t_node	*subshell_node;
	t_node	*list_node;

	subshell_node = create_node(AST_SUBSHELL);
	if (!subshell_node)
		return (0);
	append_child_node(parent, subshell_node);
	parser->advance(parser);
	list_node = parse_list(parser, subshell_node);
	if (!list_node)
		return (0);
	append_child_node(subshell_node, list_node);
	 parser->advance(parser); // For ')', need to check where R_PAREN is advanced
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
	const t_parser_fn parser_fn_arr[8] = {
		parse_err,
		0,
		parse_command,
		0,
		parse_redir_list,
		parse_pipeline,
		parse_list,
		parse_subshell,
		// parse_err,
		// parse_env_var,
		// parse_command,
		// parse_word,
		// parse_redir_list,
		// parse_pipeline,
		// parse_list,
		// parse_subshell,
	};

	root = 0;
	parse_state = LIST;
	parser.tokens = tokens;
	parser.size = num_tokens;
	parser.cur = 0;
	parser.check = &check;
	parser.cur_type = &cur_type;
	parser.advance = &advance;
	parser.peek = &peek;
	parser.consume = &consume;
	parser.is_word = &is_word_token;
	parser.is_redir = &is_redir_token;
	table = init_rule_table();
	if (!table)
		return (0);
	while (parser.cur < parser.size)
	{
		update_p_state(table, &parser, &parse_state);
		if (parse_state == ERR)
		{
			// deal with err and free?
			return (0);
		}
		// TODO: edit parser_fn_arr, so parse_state 1, 3, 7 is removed from the array, parse_state and parsing table
		else if (parse_state == PARSE_STATES_CNT || parse_state == 1 || parse_state == 3 || parse_state == 7)
		{
			break ;
		}
		new_node = parser_fn_arr[parse_state](&parser, root);
		if (new_node != 0)
		{
			// if (check_err_node(new_node))
			// {
			// 	if (root && root != new_node)
			// 		free_ast(new_node);
			// 	free_table(table);
			// 	// free ast here?
			// 	return (0);
			// }
			if (root == 0)
			{
				root = new_node;
			}
			// else if (new_node->type == AST_ERR)
			// {
			// 	// handle err, print err msg and error happend around which token
			// 	// For now, print error here and return 0;
			// 	write(STD_ERR, "minishell: syntax error near unexpected token `", 47);
			// 	write(STD_ERR, new_node->cmd_args[0], ft_strlen(new_node->cmd_args[0]));
			// 	write(STD_ERR, "`\n", 2);
			// 	free_ast(new_node);
			// 	return (0);
			// }
			else if (new_node->type == AST_REDIR_IN || new_node->type == AST_REDIR_OUT || new_node->type == AST_REDIR_APPEND || new_node->type == AST_HEREDOC)
				append_redir_node(root, new_node);
			else
				append_child_node(root, new_node);
		}
		else
		{
			return (0);
			// syntax err or malloc err?
		}
	}
	free_table(table);
	return (root);
}
