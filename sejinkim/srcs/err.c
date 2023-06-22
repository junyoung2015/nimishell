/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:51:08 by sejinkim          #+#    #+#             */
/*   Updated: 2023/06/22 10:35:58 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tree(t_node *root)
{
	if (!root)
		return ;
	free_tree(root->left);
	free_tree(root->right);
	free(root->val);
	free(root);
}

void	_free_node_(t_node *node)
{
	t_node	*prev;

	while (node)
	{
		free(node->val);
		prev = node;
		node = node->right;
		free(prev);
	}
}

void	_syntax_err_(t_node *root)
{
	free_tree(root);
	write(1, "error: syntax error\n", 20);
	exit(EXIT_FAILURE);
}
