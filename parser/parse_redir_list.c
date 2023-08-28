/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:56:38 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 13:06:53 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*parse_heredoc_cmd(t_parser *parser, t_node *parent, t_node *heredoc_node)
{
	t_node	*cmd_node;

	cmd_node = parse_simple_cmd_element(parser, parent);
	if (!cmd_node)
		return (0);
	append_child_node(cmd_node, heredoc_node);
	return (cmd_node);
}

t_node	*parse_heredoc(t_parser *parser, t_node *parent)
{
	t_node	*heredoc_node;

	heredoc_node = create_node(AST_HEREDOC);
	if (!heredoc_node)
		return (0);
	heredoc_node->cmd_args = ft_calloc(2, sizeof(char *));
	if (!heredoc_node->cmd_args)
	{
		free(heredoc_node);
		return (0);
	}
	if (is_word_token(parser))
		heredoc_node->cmd_args[(heredoc_node->num_args)++] = parse_word(parser);
	else
	{
		free(heredoc_node->cmd_args);
		free(heredoc_node);
		return (p_err(parser, parent));
	}
	advance(parser);
	if (is_word_token(parser))
		return (parse_heredoc_cmd(parser, parent, heredoc_node));
	return (heredoc_node);
}

/**
 * @brief Parse function for <REDIRECTION>. '<, >, >>, <<' should be the root
 *		node, and filename should be at the left node (<, <<) or right (>, >>).
 *		Bash only consider the first redirection-in, ignoring the rest, and
 *		the last redirection-out. So, we should consider the same.
 *
 * @param parser 	paresr struct
 * @return t_node*	root node of the <REDIRECTION>
 */
t_node	*parse_redir(t_parser *parser, t_node *parent)
{
	t_node			*redir_node;
	t_type	type;

	redir_node = 0;
	if (is_redir_token(parser))
	{
		type = cur_type(parser);
		advance(parser);
		if (type == TOKEN_HEREDOC)
			return (parse_heredoc(parser, parent));
		else
		{
			if (is_word_token(parser))
			{
				redir_node = parse_word_list(parser, parent);
				redir_node->type = (t_node_type) (type - REDIR);
			}
			else
				redir_node = p_err(parser, parent);
		}
	}
	return (redir_node);
}

/**
 * @brief Parse function for <REDIRECTION-LIST-TAIL>, calling <REDIRECTION> or
 *		indicate the end of the <REDIRECTION-LIST-TAIL>.
 * 
 * @param parser	parser struct
 * @return t_node*	root node of <REDIRECTION-LIST-TAIL>
 */
t_node	*p_redir_l_tail(t_parser *parser, t_node *parent)
{
	t_node	*redir_node;
	t_node	*redir_list_tail_node;

	redir_list_tail_node = 0;
	if (is_redir_token(parser))
	{
		redir_list_tail_node = parse_redir(parser, parent);
		if (!redir_list_tail_node)
			return (0);
		append_child_node(parent, redir_list_tail_node);
		if (is_redir_token(parser))
		{
			redir_node = p_redir_l_tail(parser, redir_list_tail_node);
			if (!redir_node)
				return (0);
		}
	}
	return (redir_list_tail_node);
}

/**
 * @brief Parse function for <REDIRECTION-LIST>, calling <REDIRECTION> and
 * 		<REDIRECTION-LIST-TAIL>.
 * 
 * @param parser	parser struct
 * @return t_node*	root node of <REDIRECTION-LIST>
 */
t_node	*p_redir_l(t_parser *parser, t_node *parent)
{
	t_node	*redir_list_node;
	t_node	*redir_list_tail_node;

	redir_list_node = parse_redir(parser, parent);
	if (!redir_list_node)
		return (0);
	else if (redir_list_node->type == AST_ERR)
		return (redir_list_node);
	if (is_redir_token(parser))
	{
		redir_list_tail_node = p_redir_l_tail(parser, redir_list_node);
		if (!redir_list_tail_node)
			return (0);
	}
	return (redir_list_node);
}
