/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmp_func_quote_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 11:40:38 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:56:57 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
