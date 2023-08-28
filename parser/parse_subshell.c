/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:56:45 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 12:56:45 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parse function for <SUBSHELL>, calling <LIST>
 *
 * @param parser	parser struct
 * @return t_node*	root node of <SUBSHELL>
 */
t_node *p_sub(t_parser *parser, t_node *parent)
{
	t_node	*list_node;
	t_node	*subshell_node;

	subshell_node = create_node(AST_SUBSHELL);
	if (!subshell_node)
		return (0);
	append_child_node(parent, subshell_node);
	advance(parser);
	list_node = p_l(parser, subshell_node);
	if (!list_node)
		return (0);
	append_child_node(subshell_node, list_node);
	advance(parser);
	return (subshell_node);
}

t_node	*parse_subshell_list(t_parser *parser, t_node *parent)
{
	t_node	*cmd_node;
	t_node	*redir_list_node;
	
	cmd_node = p_sub(parser, parent);
	if (!cmd_node)
		return (0);
	if (is_redir_token(parser))
	{
		redir_list_node = p_redir_l(parser, cmd_node);
		if (!redir_list_node)
			return (0);
		append_child_node(cmd_node->left, redir_list_node);
	}
	return (cmd_node);
}
