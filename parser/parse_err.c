#include "minishell.h"
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
    t_node *err_node;

	err_node = 0;
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
	if (parser->tokens[parser->cur].val)
		err_node->cmd_args[err_node->num_args++] = ft_strdup(parser->tokens[parser->cur].val);
	else
		err_node->cmd_args[err_node->num_args++] = ft_strdup(token_type_to_str(parser->tokens[parser->cur - 1].type));
	return (err_node);
}

char	*token_type_to_str(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return ("|");
	else if (type == TOKEN_REDIR_IN)
		return ("<");
	else if (type == TOKEN_REDIR_OUT)
		return (">");
	else if (type == TOKEN_APPEND)
		return (">>");
	else if (type == TOKEN_HEREDOC)
		return ("<<");
	return ("UNKNOWN");
}

