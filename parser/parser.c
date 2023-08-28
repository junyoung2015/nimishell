/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ll.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 19:05:20 by jusohn            #+#    #+#             */
/*   Updated: 2023/07/23 17:15:43 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Initialize the parsing table
 *
 * @return char** table of parsing table
 */
void	init_parser(t_node **root, t_parser *parser, t_token *tokens, t_size num_tokens)
{
	*root = 0;
	parser->tokens = tokens;
	parser->size = num_tokens;
	parser->cur = 0;
}

t_node	*append_node(t_node *root, t_node *node)
{
	if (check_err_node(node))
	{
		if (root && root != node)
			free_ast(root);
		free_ast(node);
		return (0);
	}
	if (root == 0)
		root = node;
	else if (AST_REDIR_IN <= node->type && AST_REDIR_APPEND <= node->type)
		append_redir_node(root, node);
	else
		append_child_node(root, node);
	return (root);
}

/**
 * @brief LL(1) recursive decent parser, which receives array of t_token
 * 		and generate an AST (Abstract Syntax Tree).
 *
 * @param tokens		array of tokens after lexical analysis
 * @param num_tokens	length of the array
 * @return t_node*		root node of AST generated
 */
t_node *parse_tokens(t_token *tokens, t_size num_tokens)
{
	t_parse_state	parse_state;
	t_node			*root;
	t_node			*node;
	t_parser		parser;
	const t_parse	fn_arr[8] = {p_err,p_cmd,p_redir_l,p_pipe_l,p_l,p_sub};

	init_parser(&root, &parser, tokens, num_tokens);
	while (parser.cur < parser.size)
	{
		update_p_state(&parser, &parse_state);
		if (parse_state == PARSE_STATES_CNT)
			break ;
		node = fn_arr[parse_state](&parser, root);
		if (node != 0)
			root = append_node(root, node);
		else
		{
			if (root)
				free_ast(root);
			return (0);
		}
	}
	return (root);
}
