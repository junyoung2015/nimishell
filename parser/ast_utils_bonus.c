/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:56:18 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:57:36 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_node	*create_node(t_node_type type)
{
	t_node	*new_node;

	new_node = (t_node *)ft_calloc(1, sizeof(t_node));
	if (!new_node)
		return (0);
	new_node->type = type;
	new_node->cmd_args = 0;
	new_node->num_args = 0;
	new_node->left = 0;
	new_node->right = 0;
	new_node->pipe_open = 0;
	new_node->builtin = NOT_BUILTIN;
	return (new_node);
}

void	free_ast(t_node *root)
{
	t_size	idx;

	if (root == 0)
		return ;
	if (root->left)
		free_ast(root->left);
	if (root->right)
		free_ast(root->right);
	if (root->cmd_args)
	{
		idx = 0;
		while (idx < root->num_args)
			free(root->cmd_args[idx++]);
		free(root->cmd_args);
	}
	free(root);
}

void	append_child_node(t_node *parent, t_node *child)
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
