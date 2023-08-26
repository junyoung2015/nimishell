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

/**
 * @brief Parse function for <WORD-LIST-TAIL>, calling <WORD> until the next
 * 		token is not a <WORD>.
 * 
 * @param parser 	parser struct
 * @return t_node*	root node of the <WORD-LIST-TAIL>
 */
t_node	*parse_word_list(t_parser *parser, t_node *parent)
{
	t_node	*word_list_node;

	word_list_node = create_node(AST_WORD_LIST);
	if (!word_list_node)
		return (0);
	word_list_node->cmd_args = ft_calloc(parser->size - parser->cur + 2, sizeof(char *));
	if (!word_list_node->cmd_args)
	{
		free(word_list_node);
		return (0);
	}
	else if (!parser->tokens[parser->cur].val)
	{
		free(word_list_node->cmd_args);
		free(word_list_node);
		return (p_err(parser, parent));
	}
	while (parser->cur < parser->size && is_word_token(parser))
	{
		word_list_node->cmd_args[word_list_node->num_args] = parse_word(parser);
		if (!word_list_node->cmd_args[word_list_node->num_args])
		{
			for (t_size i = 0; i < word_list_node->num_args; i++)
				free(word_list_node->cmd_args[i++]);
			free(word_list_node);
			return (0);
		}
		if (word_list_node->cmd_args[word_list_node->num_args])
			is_builtin_node(word_list_node);
		word_list_node->num_args++;
		advance(parser);
	}
	return (word_list_node);
}
