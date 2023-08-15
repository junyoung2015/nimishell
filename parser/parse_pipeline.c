/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:56:33 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 12:56:35 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parse function for <PARSE-PIPELINE-TAIL>, calling <COMMAND> and
 * 		<PIPELINE-TAIL>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of <PIPELINE-TAIL>, '|'.
 */
t_node *parse_pipeline_tail(t_parser *parser, t_node *parent)
{
	t_node	*right_node;
	t_node	*pipe_node;

	// do I need to check whether token == TOKEN_PIPE again?
	pipe_node = 0;
	if (check(parser, TOKEN_PIPE))
	{
		advance(parser);
		right_node = parse_pipeline(parser, parent);
		if (!right_node) // err?
			return (0);
		else if (right_node->type == AST_ERR)
			return (right_node);
		pipe_node = create_node(AST_PIPE);
		if (!pipe_node)	// malloc err
			return (0);
		pipe_node->right = right_node;
		return (pipe_node);
	}
	else	// syntax error
		return (0);
	return (pipe_node);
}

/**
 * @brief Parse function for <PIPELINE>, calling <COMMAND> or <PIPELINE-TAIL>.
 *
 * @param parser	parser struct
 * @return t_node*	root node of <PIPELINE>
 */
t_node *parse_pipeline(t_parser *parser, t_node *parent)
{
	t_node	*cmd_node;
	// t_node	*right_node;
	t_node	*pipe_node;

	cmd_node = parse_command(parser, parent);
	if (!cmd_node) // err? no command before '|'
		return (0);
	else if (cmd_node->type == AST_ERR)
		return (cmd_node);
	if (check(parser, TOKEN_PIPE))
	{
		pipe_node = parse_pipeline_tail(parser, cmd_node);
		if (!pipe_node) // err?
			return (0);
		if (pipe_node->type == AST_ERR)
		{
			free_ast(cmd_node);
			return (pipe_node);
		}
		pipe_node->left = cmd_node;
		return (pipe_node);
	}
	return (cmd_node);
}
