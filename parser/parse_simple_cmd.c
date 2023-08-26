/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:56:42 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 13:06:53 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parse function for <SIMPLE-COMMAND-ELEMENT>, calling either <WORD>
 * 		or <REDIRECTION>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of the <SIMPLE-COMMAND-ELEMENT>
 */
t_node	*parse_simple_cmd_element(t_parser *parser, t_node *parent)
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
		cmd_element = p_redir_l(parser, parent);
		if (!cmd_element)
			return (0);
	}
	else
		cmd_element = p_err(parser, parent);
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
		}
	}
	else
		cmd_node = p_err(parser, parent);
	return (cmd_node);
}

/**
 * @brief Parse function for <COMMAND>, calling <SIMPLE-COMMAND> or
 * 		<SUBSHELL>
 *
 * @param parser	parser struct
 * @return t_node*	root node from parse_simple_cmd or p_sub
 */
t_node	*p_cmd(t_parser *parser, t_node *parent)
{
	t_node	*cmd_node;
	t_node	*redir_list_node;
	t_type	state;

	state = cur_type(parser);
	if (TOKEN_L_PAREN == state)
	{
		cmd_node = p_sub(parser, parent);
		if (!cmd_node)
			return (0);
		if (is_redir_token(parser))
		{
			redir_list_node = p_redir_l(parser, cmd_node);
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
		cmd_node = p_err(parser, parent);
	}
	else
		cmd_node = p_err(parser, parent);
	return (cmd_node);
}
