/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 14:42:43 by sejinkim          #+#    #+#             */
/*   Updated: 2023/06/22 11:00:56 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	_print_parser_(t_node *node, int level)
{
	if (!node)
		return ;
	printf("level %d, <%s>\n", level, node->val);
	_print_parser_(node->left, level + 1);
	_print_parser_(node->right, level + 1);
}

void	_tree_(t_node **head, int *err_code)
{
	t_node	*root;
	t_node	*prev;

	root = *head;
	while (root && *(root->val) != '|')
	{
		prev = root;
		root = root->right;
	}
	if (!root)
		return ;
	prev->right = NULL;
	root->left = *head;
	*head = root;
	_tree_(&root->right, err_code);
	if (!root->right)
		*err_code = 1;
}

void	parser(t_node **head)
{
	int	syntax_err;

	syntax_err = 0;
	_tree_(head, &syntax_err);
	if (syntax_err)
		_syntax_err_(*head);
	_print_parser_(*head, 0);
}
