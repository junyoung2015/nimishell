#include "minishell.h"

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
	advance(parser);
	list_node = parse_list(parser, subshell_node);
	if (!list_node)
		return (0);
	append_child_node(subshell_node, list_node);
	 advance(parser); // For ')', need to check where R_PAREN is advanced
	return (subshell_node);
}
