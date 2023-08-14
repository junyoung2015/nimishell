#include "minishell.h"

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

	if (is_word_token(parser))
	{
		cmd_element = parse_word_list(parser, parent);
		if (!cmd_element)
			return (0);
		cmd_element->type = AST_CMD;
	}
	else if (is_redir_token(parser))
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
	if (is_word_token(parser) || is_redir_token(parser))
	{
		simple_cmd_tail_node = parse_simple_cmd_element(parser, parent);
		if (!simple_cmd_tail_node)
			return (0);
		if (AST_REDIR_IN <= simple_cmd_tail_node->type && simple_cmd_tail_node->type <= AST_REDIR_APPEND) // appending to the first cmd when there is a redirection - ls | wc > a.txt, appending to ls here
			append_redir_node(parent, simple_cmd_tail_node);
		else
			append_child_node(parent, simple_cmd_tail_node);
		if (is_word_token(parser) || is_redir_token(parser))
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

	if (is_word_token(parser) || is_redir_token(parser))
	{
		cmd_node = parse_simple_cmd_element(parser, parent);
		if (!cmd_node)
			return (0);
		if (is_word_token(parser) || is_redir_token(parser))
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
	// advance(parser);
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

	state = cur_type(parser);
	if (TOKEN_L_PAREN == state)
	{
		cmd_node = parse_subshell(parser, parent);
		if (!cmd_node)
			return (0);
		if (is_redir_token(parser))
		{
			redir_list_node = parse_redir_list(parser, cmd_node);
			if (!redir_list_node)
				return (0);
			append_child_node(cmd_node, redir_list_node);
		}
	}
	else if (is_word_token(parser) || is_redir_token(parser))
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
