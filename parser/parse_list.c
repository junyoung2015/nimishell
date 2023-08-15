#include "minishell.h"


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

	logic_node = 0;
	state = AST_AND;
	if (check(parser, TOKEN_OR))
		state = AST_OR;
	logic_node = create_node(state);
	if (!logic_node)	// malloc err
		return (0);
	advance(parser);
	pipeline_node = parse_pipeline(parser, logic_node);
	if (!pipeline_node)
	{
		// free and handle err here?
		free(logic_node);
		return (0);
	}
	if (pipeline_node->type == AST_ERR)
	{
		free_ast(logic_node);
		return (pipeline_node);
	}
	if (check(parser, TOKEN_OR))
	{
		list_tail_node = parse_list_tail(parser, pipeline_node);
		if (!list_tail_node) // err?
			return (0);
		// append_child_node(logic_node, parent);
		// append_child_node(logic_node, list_tail_node);
		// append_child_node(list_tail_node, pipeline_node);
		logic_node->left = parent;
		logic_node->right = list_tail_node;
		// list_tail_node->left = pipeline_node;
	}
	else if (check(parser, TOKEN_AND))
	{
		list_tail_node = parse_list_tail(parser, pipeline_node);
		if (!list_tail_node) // err?
			return (0);
		// append_child_node(logic_node, parent);
		// append_child_node(logic_node, pipeline_node);
		// append_child_node(list_tail_node, pipeline_node);
		logic_node->left = parent;
		logic_node->right = pipeline_node;
		list_tail_node->left = logic_node;
		return (list_tail_node);
	}
	else
	{
		logic_node->left = parent;
		logic_node->right = pipeline_node;
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
	if (check(parser, TOKEN_AND) || check(parser, TOKEN_OR))
	{
		list_tail_node = parse_list_tail(parser, pipeline_node);
		if (!list_tail_node) // err?
			return (0);
		// list_tail_node->left = pipeline_node;
		return (list_tail_node);
	}
	// else if (check(parser, TOKEN_AND))
	// {
	// }
	return (pipeline_node);
}
