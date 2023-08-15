/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp_func_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 11:40:38 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/15 12:51:07 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_bool	is_squote(char ch)
{
	return (ch == '\'');
}

t_bool	is_dquote(char ch)
{
	return (ch == '"');
}

t_bool	is_quote(char ch)
{
	return (is_squote(ch) || is_dquote(ch));
}

/**
 * @brief Return corresponding compare function 'cmp' for 'ch'.
 * 
 * @param ch		cmp function
 * @return t_cmp	opposite cmp function
 */
t_cmp	get_cmp_fn(char ch)
{
	if (is_squote(ch))
		return (is_squote);
	return (is_dquote);
}
