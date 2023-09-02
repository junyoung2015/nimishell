/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp_func_meta_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 12:51:11 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:56:55 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

t_bool	is_meta(char ch)
{
	return (ch == '|' || ch == '>' || ch == '<' || ch == '(' \
		|| ch == ')' || ch == '&' || is_space(ch));
}

t_bool	is_dmeta_ch(char ch)
{
	return (ch == '|' || ch == '>' || ch == '<' || ch == '&');
}

t_bool	is_space(char ch)
{
	return (ch == ' ' || ch == '\t' || ch == '\f' || ch == '\n' || \
		ch == '\v' || ch == '\r');
}

t_bool	is_not_space(char ch)
{
	return (!is_space(ch));
}

/**
 * @brief Check whether 'input' matches double-character operators.
 * 
 * @param input 
 * @return t_type
 * 		enum, but also works as TRUE or FALSE, since TOKEN_WORD = 0,
 * 		and other enums are above 0.
 */
int	is_dmeta_str(char *input)
{
	if (!(is_dmeta_ch(0[input]) && is_dmeta_ch(1[input])))
		return (TOKEN_WORD);
	if (!ft_strncmp(input, HEREDOC, 2))
		return (TOKEN_HEREDOC);
	else if (!ft_strncmp(input, APPEND, 2))
		return (TOKEN_APPEND);
	else if (!ft_strncmp(input, OR, 2))
		return (TOKEN_OR);
	else if (!ft_strncmp(input, AND, 2))
		return (TOKEN_AND);
	return (TOKEN_WORD);
}
