
#include "minishell.h"

t_node *create_node(t_node_type type)
{
	t_node *new_node;

	new_node = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!new_node)
		return (0);
	new_node->type = type;
	new_node->cmd_args = 0;
	new_node->num_args = 0;
	new_node->left = 0;
	new_node->right = 0;
	new_node->pipe_open = 0;
	new_node->parent_type = AST_NULL;
	new_node->builtin = NOT_BUILTIN;
	return (new_node);
}

void free_ast(t_node *root)
{
	if (root == 0)
		return ;
	if (!root->left)
		free_ast(root->left);
	if (!root->right)
		free_ast(root->right);
	if (root->cmd_args)
	{
		for (t_size i = 0; i < root->num_args; i++)
			free(root->cmd_args[i]);
		free(root->cmd_args);
	}
	free(root);
}

void append_child_node(t_node *parent, t_node *child)
{
	if (!parent || !child || parent == child)
		return ;
	if (!parent->left)
		parent->left = child;
	else if (!parent->right)
		parent->right = child;
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
	if (!parent || !child)
		return ;
	if (child->type == AST_REDIR_IN || child->type == AST_HEREDOC)
	{
		if (!parent->left)
			parent->left = child;
	}
	else if (child->type == AST_REDIR_OUT || child->type == AST_REDIR_APPEND)
	{
		if (!parent->right)
			parent->right = child;
	}
}
