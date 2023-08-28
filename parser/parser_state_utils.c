/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:56:56 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:18:04 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Check the type of the current token.
 * 
 * @param parser 		parser struct
 * @return t_type	type of the current token
 */
t_type	cur_type(t_parser *parser)
{
	if (parser->cur < parser->size)
		return (parser->tokens[parser->cur].type);
	return (TOKEN_TYPES_CNT);
}

/**
 * @brief Check the current token matches with t_type passed.
 *
 * @param parser	parser struct
 * @param type		type of the token to compare
 * @return t_bool
 */
t_bool	check(t_parser *parser, t_type type)
{
	if (parser->cur >= parser->size)
		return (FALSE);
	return (cur_type(parser) == type);
}

/**
 * @brief Return the type of the token right after the current index. If
 * 		current token is the last one, return TOKENS_TYPES_CNT(20).
 *
 * @param parser		parser struct
 * @return t_type	type of the next token
 */
t_type	peek(t_parser *parser)
{
	if (parser->cur < parser->size)
		return (parser->tokens[parser->cur + 1].type);
	return (TOKEN_TYPES_CNT);
}

/**
 * @brief Increase the current index, after checking the range.
 *
 * @param parser	parser struct
 */
void	advance(t_parser *parser)
{
	if (parser->cur < parser->size)
		parser->cur++;
}
