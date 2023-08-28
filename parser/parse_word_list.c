/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_word_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:56:48 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 12:56:50 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Parse function for <WORD>. For <REDIRECTION>, <WORD> should be
 * 		attached to left (<, <<) node or right (>, >>) of the parent node.
 * 		For	<WORD> as an argument of a command, so should be directly attached
 * 		to parent->cmd_args[0 < i].
 *		If previous token is <WORD>, it should be considered as arguments,
 *		attaching arugments to (parent->cmd_args[0 < i]) and return
 *		(parent).
 *
 * @param parser	paresr struct
 * @return t_node*	root node of the <REDIRECTION>, or <WORD>
 */
char *parse_word(t_parser *parser)
{
	char	*word;

	word = 0;
	if (is_word_token(parser))
		word = ft_strdup(parser->tokens[parser->cur].val);
	return (word);
}

t_node	*parse_each_word(t_parser *parser, t_node *parent, t_node *list)
{
	t_size	idx;

	(void) parent;
	idx = 0;
	list->cmd_args[list->num_args] = parse_word(parser);
	if (!list->cmd_args[list->num_args])
	{
		while (idx < list->num_args)
			free(list->cmd_args[idx++]);
		free(list);
		return (0);
	}
	if (list->cmd_args[list->num_args])
		is_builtin_node(list);
	list->num_args++;
	advance(parser);
	return (list);
}

/**
 * @brief Parse function for <WORD-LIST-TAIL>, calling <WORD> until the next
 * 		token is not a <WORD>.
 * 
 * @param parser 	parser struct
 * @return t_node*	root node of the <WORD-LIST-TAIL>
 */
t_node	*parse_word_list(t_parser *parser, t_node *parent)
{
	t_node	*list;

	list = create_node(AST_WORD_LIST);
	if (!list)
		return (0);
	list->cmd_args = ft_calloc(parser->size - parser->cur + 2, sizeof(char *));
	if (!list->cmd_args)
	{
		free(list);
		return (0);
	}
	else if (!parser->tokens[parser->cur].val)
	{
		free(list->cmd_args);
		free(list);
		return (p_err(parser, parent));
	}
	while (parser->cur < parser->size && is_word_token(parser))
	{
		list = parse_each_word(parser, parent, list);
		if (!list)
			return (0);
	}
	return (list);
}
