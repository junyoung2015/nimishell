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
	advance(parser);
	return (subshell_node);
}
